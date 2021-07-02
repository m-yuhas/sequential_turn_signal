/* Gasket and screws for the enclosure for:
 * Sequential Turn Signal
 *
 * Ideally the equivalent screws are purchased an not self-fabricated
 * (ANSI/ASME B1.13M-1995 M2.2 x 0.25).  Ideally a silicon gasket will be used.
 */

include <MCAD/screw.scad>

inner_x = 160;          // Interior dimension of enclosure in x direction.
inner_y = 100;          // Interior dimension of enclosure in y direction.
gasket_d = 4;           // Minimum gasket thickness along the the x/y plane.
gasket_z = 1;           // Gasket height in z direction.
mount_r = 3;            // Radius of PCB mounting posts.
screw_inset = 6;        // Distance from closest edge of PCB to center of screw.
screw_r = 3;            // Screw head radius.
screw_h = 2;            // Height of screw head.
thread_h = 20;          // Height of the screw shaft.
thread_pitch = 0.25;    // Mounting screw thread pitch.
thread_outer_r = 2.182; // Mounting screw outer radius.
thread_inner_r = 1.911; // Mounting screw inner radius.
pwr_inset_x = 11;       // Size of power connector in x direction.
pwr_inset_y = 23;       // Size of power connector in the y direction.
pwr_offset = 13;        // Offset from origin along y axis to power connector.
sig_inset_x = 11;       // Size of signal connector in x direction.
sig_inset_y = 33;       // Size of signal connector in y direction.
sig_offset = 53;        // Offset from origin along y axis to signal connector.
eps = 0.001;            // Epsilon constant to ensure overlap for difference().

// Gasket to provide a seal between the top and bottom of the enclosure.
module gasket()
{

  // Exterior shell of the gasket.
  module exterior()
  {
    difference()
    {
      cube([inner_x + 2 * gasket_d, inner_y + 2 * gasket_d, gasket_z]);
      translate([-eps, pwr_offset + gasket_d, -eps]) {
        cube([pwr_inset_x + eps, pwr_inset_y, gasket_z + 2 * eps]);
      }
      translate([-eps, sig_offset + gasket_d, -eps])
      {
        cube([sig_inset_x + eps, sig_inset_y, gasket_z + 2 * eps]);
      }
    }
  }

  // Interior shell of the gasket.
  module interior()
  {
    difference()
    {
      cube([inner_x, inner_y, gasket_z + 2 * eps]);
      translate([-eps, pwr_offset - gasket_d, -2 * eps])
      {
        cube([pwr_inset_x + eps, pwr_inset_y + 2 * gasket_d, gasket_z + 4 * eps]);
      }
      translate([-eps, sig_offset - gasket_d, -2 * eps]) {
        cube([sig_inset_x + eps, sig_inset_y + 2 * gasket_d, gasket_z + 4 * eps]);
      }
    }
  }

  // Construct gasket by removing the interior volume from the exterior volume.
  difference()
  {
    exterior();
    translate([gasket_d, gasket_d, -eps]) interior();
  }

}

// Example of a screw compatible with this enclosure.
module screw()
{

  // Screw head.
  module head()
  {
    difference()
    {
      cylinder(r=screw_r, h=screw_h);
      translate([-screw_r - eps, -screw_h / 2, -eps]) {
        cube([2 * screw_r + 2 * eps, screw_h, screw_h / 2 + eps]);
      }
    }
  }

  // Screw shaft.
  module shaft()
  {
    translate([0, 0, screw_h - eps]) {
      auger(pitch=thread_pitch,
            length=thread_h + eps,
            outside_radius=thread_outer_r,
            inner_radius=thread_inner_r);
    }
  }

  // The union of the head and shaft compose the screw.
  head();
  shaft();

}

gasket();
translate([gasket_d + screw_inset, gasket_d + screw_inset, 0]) screw();
translate([inner_x + gasket_d - screw_inset, gasket_d + screw_inset, 0]) screw();
translate([gasket_d + screw_inset, inner_y + gasket_d - screw_inset, 0]) screw();
translate([inner_x + gasket_d - screw_inset, inner_y + gasket_d - screw_inset, 0]) screw();
