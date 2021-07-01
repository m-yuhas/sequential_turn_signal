/* Top portion of the enclosure for:
 * Sequential Turn Signal
 *
 * This part can be constructed out of metal, plastic, or wood,
 * but metal is recommended.  The enclosure should be mounted in
 * a dry, well-ventilated area as it will not prevent moisture
 * ingress or provide thermal insulation for the electronics.
 */

inner_x = 160;          // Longest interior length of the gasket
inner_y = 100;
inner_z = 20;          // Longest interior width of the gasket
wall_d = 4;           // Thickness of gasket along x/y axis
con1_inset_x = 11;      // Length of power header perpendicular to PCB edge
con1_inset_y = 23;      // Length of power header parallel to PCB edge
con1_offset_y = 13;     // Offset from origin to the power header
con2_inset_x = 11;      // Length of signal header perpendicular to PCB edge
con2_inset_y = 33;     // Length of signal header parallel to PCB edge
con2_offset_y = 53;      // Offset from origin to the signal header

screw_head_r = 3;
screw_head_h = 2;
screw_inset = 6;


module top() {
    module outline() {
        difference() {
            cube([inner_x + 2 * wall_d, inner_y + 2 * wall_d, inner_z + wall_d]);
            translate([-1, con1_offset_y + wall_d, -1]) {
                cube([con1_inset_x + 1, con1_inset_y, inner_z + wall_d + 2]);
            }
            translate([-1, con2_offset_y + wall_d, -1]) {
                cube([con2_inset_x + 1, con2_inset_y, inner_z + wall_d + 2]);
            }
        }
    }
    module interior() {
        translate([wall_d, wall_d, -1]) {
            difference() {
                cube([inner_x, inner_y, inner_z + 1]);
                translate([-1, con1_offset_y - wall_d, -2]) {
                    cube([con1_inset_x + 1, con1_inset_y + 2 * wall_d, inner_z + 4]);
                }
                translate([-1, con2_offset_y - wall_d, -2]) {
                    cube([con2_inset_x + 1, con2_inset_y + 2 * wall_d, inner_z + 4]);
                }
            }
        }
    }
    difference() {
        outline();
        interior();
    }   
}

top();