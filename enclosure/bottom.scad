/* Bottom portion of the enclosure for:
 * Sequential Turn Signal
 *
 * This part can be constructed out of metal, plastic, or wood,
 * but metal is recommended.  The enclosure should be mounted in
 * a dry, well-ventilated area as it will not prevent moisture
 * ingress or provide thermal insulation for the electronics.
 */

include <MCAD/screw.scad>

inner_length = 160;
inner_width = 100;
inner_height = 10;
wall_thickness = 5;
mount_radius = 3;
washer_thickness = 1;
pcb_screw_inset = 6;


// Draw the bottom and sides
cube([inner_length + 2 * wall_thickness,
      inner_width + 2 * wall_thickness,
      wall_thickness]);

translate([0, 0, wall_thickness]) {
    cube([inner_length + 2 * wall_thickness, wall_thickness, inner_height]);
    cube([wall_thickness, inner_width + 2 * wall_thickness, inner_height]);
}

translate([inner_length + wall_thickness, 0, wall_thickness]) {
    cube([wall_thickness, inner_width + 2 * wall_thickness, inner_height]);
}

translate([0, inner_width + wall_thickness, wall_thickness]) {
    cube([inner_length + wall_thickness, wall_thickness, inner_height]);
}


// Draw the PCB mounting points
module threaded_mount() {
    difference() {
        cylinder(r=mount_radius, h=inner_height - washer_thickness);
        auger(pitch=0.25, length=10, outside_radius=2.182, inner_radius=1.911);
    }
}

translate([wall_thickness + pcb_screw_inset,
           wall_thickness + pcb_screw_inset,
           wall_thickness]) {
    threaded_mount();
}

translate([inner_length + wall_thickness - pcb_screw_inset,
           wall_thickness + pcb_screw_inset,
           wall_thickness]) {
    threaded_mount();
}

translate([wall_thickness + pcb_screw_inset,
           inner_width + wall_thickness - pcb_screw_inset,
           wall_thickness]) {
    threaded_mount();
}

translate([inner_length + wall_thickness - pcb_screw_inset,
           inner_width + wall_thickness - pcb_screw_inset,
           wall_thickness]) {
    threaded_mount();
}
