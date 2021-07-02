/* Bottom portion of the enclosure for:
 * Sequential Turn Signal
 *
 * This part can be constructed out of metal, plastic, or wood,
 * but metal is recommended.  The enclosure should be mounted in
 * a dry, well-ventilated area as it will not prevent moisture
 * ingress or provide thermal insulation for the electronics.
 */

include <MCAD/screw.scad>

inner_x = 160;          // Interior dimension of enclosure in x direction.
inner_y = 100;          // Interior dimension of enclosure in y direction.
inner_z = 10;           // Interior dimension of enclosure in z direction.
wall_d = 4;             // Enclosure wall thickness.
mount_r = 3;            // Radius of PCB mounting posts.
pcb_z = 1;              // PCB thickness.
screw_inset = 6;        // Distance from closest edge of PCB to center of screw.
thread_pitch = 0.25;    // Mounting screw thread pitch.
thread_outer_r = 2.182; // Mounting screw outer radius.
thread_inner_r = 1.911; // Mounting screw inner radius.
eps = 0.001;            // Epsilon constant to ensure overlap for difference().

// The bottom portion of the enclosure.
module bottom()
{

  // Mounting stub to attach the PCB to the bottom of the enclosure.
  module mount()
  {
    difference()
    {
      cylinder(r=mount_r, h=inner_z - pcb_z);
      auger(pitch=thread_pitch,
            length=inner_z - pcb_z + eps,
            outside_radius=thread_outer_r,
            inner_radius=thread_inner_r);
    }
  }

  // Interior volume of the bottom of the enclosure.
  module interior()
  {
    difference()
    {
      cube([inner_x, inner_y, inner_z + eps]);
      translate([screw_inset, screw_inset, 0]) mount();
      translate([inner_x - screw_inset, screw_inset, 0]) mount();
      translate([screw_inset, inner_y - screw_inset, 0]) mount();
      translate([inner_x - screw_inset, inner_y - screw_inset, 0]) mount();
    }
  }

  /* Construct the bottom portion of the enclosure by removing the interior
   * volume from the volume bounded by the exterior.
   */
  difference()
  {
    cube([inner_x + 2 * wall_d, inner_y + 2 * wall_d, inner_z + wall_d]);
    translate([wall_d, wall_d, wall_d]) interior();
  }

}

bottom();
