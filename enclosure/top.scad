/* Top portion of the enclosure for:
 * Sequential Turn Signal
 *
 * This part can be constructed out of metal, plastic, or wood,
 * but metal is recommended.  The enclosure should be mounted in
 * a dry, well-ventilated area as it will not prevent moisture
 * ingress or provide thermal insulation for the electronics.
 */

inner_x = 160;          // Interior dimension of enclosure in x direction.
inner_y = 100;          // Interior dimension of enclosure in y direction.
inner_z = 20;           // Interior dimension of enclosure in z direction.
inner_z_min = 10;        // Minimum distance between top of enclosure and PCB.
wall_d = 4;             // Enclosure wall thickness.
mount_r = 3;            // Radius of PCB mounting posts.
screw_inset = 6;        // Distance from closest edge of PCB to center of screw.
screw_r = 3;            // Screw head radius.
thread_outer_r = 2.182; // Mounting screw outer radius.
pwr_inset_x = 11;       // Size of power connector in x direction.
pwr_inset_y = 23;       // Size of power connector in the y direction.
pwr_offset = 13;        // Offset from origin along y axis to power connector.
sig_inset_x = 11;       // Size of signal connector in x direction.
sig_inset_y = 33;       // Size of signal connector in y direction.
sig_offset = 53;        // Offset from origin along y axis to signal connector.
eps = 0.001;            // Epsilon constant to ensure overlap for difference().

// The top portion of the enclosure.
module top()
{

  // Exterior shell of the top of the enclosure.
  module exterior()
  {
    difference()
    {
      cube([inner_x + 2 * wall_d, inner_y + 2 * wall_d, inner_z + wall_d]);
      translate([-eps, pwr_offset + wall_d, -eps])
      {
        cube([pwr_inset_x + eps, pwr_inset_y, inner_z + wall_d + 2 * eps]);
      }
      translate([-eps, sig_offset + wall_d, -eps]) {
         cube([sig_inset_x + eps, sig_inset_y, inner_z + wall_d + 2 * eps]);
      }
    }
  }

  // Mounting stub which contacts the PCB at the screw holes.
  module mount()
  {
    translate([0, 0, inner_z_min])
    {
      cylinder(r=screw_r + wall_d, h=inner_z_min);
    }
    cylinder(r=mount_r, h=inner_z);
  }

  // Interior volume of the top of the enclosure.
  module interior()
  {
    difference()
    {
      translate([0, 0, -eps]) cube([inner_x, inner_y, inner_z + eps]);
      translate([-eps, pwr_offset - wall_d, -2 * eps])
      {
        cube([pwr_inset_x + eps, pwr_inset_y + 2 * wall_d, inner_z + 4 * eps]);
      }
      translate([-eps, sig_offset - wall_d, -2 * eps])
      {
        cube([sig_inset_x + eps, sig_inset_y + 2 * wall_d, inner_z + 4 * eps]);
      }
      translate([screw_inset, screw_inset, 0]) mount();
      translate([inner_x - screw_inset, screw_inset, 0]) mount();
      translate([screw_inset, inner_y - screw_inset, 0]) mount();
      translate([inner_x - screw_inset, inner_y - screw_inset, 0]) mount();
    }
  }

  // Holes for screws to attach the top and bottom portions of the enclosure.
  module screw_holes()
  {

    // Screw hole centered on the origin.
    module hole()
    {
      translate([0, 0, inner_z_min + wall_d])
      {
        cylinder(r=screw_r, h=inner_z_min + wall_d);
      }
      translate([0, 0, -eps])
      {
        cylinder(r=thread_outer_r, h=inner_z + wall_d + eps);
      }
    }

    // Copy the hole at the correct locations.
    translate([screw_inset, screw_inset, 0]) hole();
    translate([inner_x - screw_inset, screw_inset, 0]) hole();
    translate([screw_inset, inner_y - screw_inset, 0]) hole();
    translate([inner_x - screw_inset, inner_y - screw_inset, 0]) hole();

  }

  /* Construct the top portion of the enclosure by removing the interior
   * volume and the screw holes from the volume bounded by the exterior.
   */
  difference()
  {
    exterior();
    translate([wall_d, wall_d, 0]) interior();
    translate([wall_d, wall_d, 0]) screw_holes();
  }

}

top();
