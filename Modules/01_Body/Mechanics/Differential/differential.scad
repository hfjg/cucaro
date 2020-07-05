// cucaro differencial

use <hg_zahnradlib.scad>
use <screws.scad>
use <baConEl.scad>
$fn=72;
eps = 0.05;

module bearing608(){
    difference(){
        cylinder(d=22,h=7, center=true);
        cylinder(d=8.2,h=7+2*eps, center=true);
    }
}

spacers();
module spacers(){
    color("green"){
        for(m=[0,1]){ mirror([m,0,0]){
            translate([27.5,0,0])rotate([0,-90,0]) spacer(0,12,5,12,8.2,1);
            
            translate([27.5+11,0,0])rotate([0,-90,0]) spacer(0,12, 4,12, 8.2,0);
        }
      mirror([0,m,0]) translate([0,24.5,0])rotate([90,0,0]) spacer(0,12,1,12,8.2,0);  
      }
    }
}

module spacer(h1=1, d1=12, h2=3, d2=12, di=8.2, chamfer=1){
    difference(){
        union(){
            cylinder(d=d1, h=h1);
            translate([0,0,h1])cylinder(d=d2, h=h2-chamfer);
            translate([0,0,h1+h2-chamfer])cylinder(d1=d2, d2=d2-2*chamfer, h=chamfer);
        }
        translate([0,0,-eps])cylinder(d=di, h=h1+h2+2*eps);
    }
}

!internalBevelGear();
module internalBevelGear(){
    difference(){
        union(){
            translate([0,0,4.5])pfeilkegelrad(modul=1, zahnzahl=40, teilkegelwinkel=45, zahnbreite=6, bohrung=2, eingriffswinkel=20, schraegungswinkel=+30);
            cylinder(d1=40-2*4.5, d2=40, h=4.5);
            cylinder(d=30.4, h=9.75);
        }
        cylinder(d=12, h=50, center=true);
        translate([0,0,-eps])cylinder(d=22.2, h=7.1);
        translate([0,0,-eps])cylinder(d1=8.2+2, d2=8.2, h=1);
    }
}

//!halfshaftBevelGear();
module halfshaftBevelGear(){
    difference(){
        union(){
            translate([0,0,4.5])pfeilkegelrad(modul=1, zahnzahl=40, teilkegelwinkel=45, zahnbreite=6, bohrung=2, eingriffswinkel=20, schraegungswinkel=-30);
            cylinder(d1=40-2*4.5, d2=40, h=4.5);
        }
        cylinder(d=8.2, h=50, center=true);
        translate([0,0,4])cylinder($fn=6, d=13/cos(30)+0.4, h=5.5);
        translate([0,0,-eps])cylinder(d1=8.2+2, d2=8.2, h=1);
        circleOf(num=8, r=11)scale([2.2,1.6,1])cylinder(d=3, h=30, center=true);
    }
}

module externalParts(){
    color("Sienna"){
        // halfshafts
        for (m=[0,1]) mirror([m,0,0]){
            translate([12,0,0])rotate([0,90,0])hexBolt(8,50);
            translate([40,0,0])rotate([0,90,0]) hexNut(8);
            
            // bearings
            translate([31,0,0])rotate([0,90,0])bearing608();

        }
        
        for (m=[0,1]) mirror([0,m,0]){
            translate([0,20,0])rotate([90,0,0])bearing608();
        }
        
        // inner shaft
        rotate([90,0,0])cylinder(d=8, h=55, center=true);
    }
}



module box(upper=1, lower=1){
	difference(){
		difference(){
            union(){
    			cube([74,62,62], center=true);
                translate([0,0,-62/2+2/2])cube([74+2*10,62,2], center=true);
            }
         
            // fixing holes
            rectOf(x=84, y=48, center=[0,0,-62/2-eps]) cylinder(d=3, h=5);   
			cube([50,50,58], center=true);
			//cube([45,45,70], center=true);

			rotate([0,90,0])cylinder(d=12, h=100, center=true);
			rotate([90,0,0])cylinder(d=8, h=62-5, center=true);

			rectOf(x=62, y=52) cylinder(d=4.2, h=100, center=true);
            
            translate([0,0,-62/2-eps])rectOf(x=62, y=52) cylinder($fn=6, d=7/cos(30)+0.2, h=20, center=false);
            translate([0,0,+62/2+eps-15])rectOf(x=62, y=52) cylinder(d=7.4, h=20, center=false);

            // bearing seats
			for (i=[-1,1]) translate([i*31,0,0]) rotate([0,90,0])cylinder(d=22.2, h=7.2, 
                center=true);
		}
	if (upper==0) translate([0,0,50])cube([100,100,100], center=true);
	if (lower==0) translate([0,0,-50])cube([100,100,100], center=true);
	}
}
color("yellow")box(0,1);

for (phi=[0,180]) rotate([0,0,phi]){
    translate([23.5,0,0])rotate([0,-90,0]) halfshaftBevelGear();
}
for (phi=[0,180]) rotate([0,0,phi]){
    translate([0,-23.5,0])rotate([-90,0,0])internalBevelGear();

}

externalParts();
