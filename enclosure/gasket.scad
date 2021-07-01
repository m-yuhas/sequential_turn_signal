/* Gasket and screws for the enclosure for:
 * Sequential Turn Signal
 *
 * Ideally the equivalent screws are purchased an not self-fabricated
 * (ANSI/ASME B1.13M-1995 M2.2 x 0.25).  Ideally a silicon gasket will be used.
 */
 
include <MCAD/screw.scad>

inner_x = 160;          // Longest interior length of the gasket
inner_y = 100;          // Longest interior width of the gasket
gasket_d = 4;           // Thickness of gasket along x/y axis
gasket_z = 1;           // Gasket height
con1_inset_x = 11;      // Length of power header perpendicular to PCB edge
con1_inset_y = 23;      // Length of power header parallel to PCB edge
con1_offset_y = 13;     // Offset from origin to the power header
con2_inset_x = 11;      // Length of signal header perpendicular to PCB edge
con2_inset_y = 33;     // Length of signal header parallel to PCB edge
con2_offset_y = 53;      // Offset from origin to the signal header

screw_head_r = 3;
screw_head_h = 2;
screw_inset = 6;


module gasket() {
    module outline() {
        difference() {
            cube([inner_x + 2 * gasket_d, inner_y + 2 * gasket_d, gasket_z]);
            translate([-1, con1_offset_y + gasket_d, -1]) {
                cube([con1_inset_x + 1, con1_inset_y, gasket_z + 2]);
            }
            translate([-1, con2_offset_y + gasket_d, -1]) {
                cube([con2_inset_x + 1, con2_inset_y, gasket_z + 2]);
            }
        }
    }
    module interior() {
        translate([gasket_d, gasket_d, -1]) {
            difference() {
                cube([inner_x, inner_y, gasket_z + 2]);
                translate([-1, con1_offset_y - gasket_d, -2]) {
                    cube([con1_inset_x + 1, con1_inset_y + 2 * gasket_d, gasket_z + 4]);
                }
                translate([-1, con2_offset_y - gasket_d, -2]) {
                    cube([con2_inset_x + 1, con2_inset_y + 2 * gasket_d, gasket_z + 4]);
                }
            }
        }
    }
    difference() {
        outline();
        interior();
    }   
}

module screw() {
    difference() {
        cylinder(r=screw_head_r, h=screw_head_h);
        translate([-screw_head_r - 1, -screw_head_h / 2, -1]) {
            cube([2 * screw_head_r + 2, screw_head_h, screw_head_h / 2 + 1]);
        }
    }
    translate([0, 0, screw_head_h]) {
        auger(pitch=0.25, length=10, outside_radius=2.182, inner_radius=1.911);
    }
}

gasket();

translate([gasket_d + screw_inset, gasket_d + screw_inset, 0]) {
    screw();
}

translate([inner_x + gasket_d - screw_inset, gasket_d + screw_inset, 0]) {
    screw();
}

translate([gasket_d + screw_inset, inner_y + gasket_d - screw_inset, 0]) {
    screw();
}

translate([inner_x + gasket_d - screw_inset, inner_y + gasket_d - screw_inset, 0]) {
    screw();
}
