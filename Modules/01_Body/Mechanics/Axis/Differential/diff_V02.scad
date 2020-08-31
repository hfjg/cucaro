use <hg_zahnradlib.scad>
use <baConEl.scad>


eps=0.05;
$fn=36;

dSchraube=4.2; swHead=7; lTube=11+10;
dHuelse=8;

shift=15.;

!diffZahnRadB(-1);


module diffZahnRadB(dir=1){
    difference(){
        cylinder(d1=26, d2=41, h=7);
        translate([0,0,-eps])cylinder(d=22.1, h=15);
    }
    translate([0,0,6.5])
    pfeilkegelrad(modul=1.5, zahnzahl=28,    teilkegelwinkel=45, zahnbreite=5, bohrung=22.1, eingriffswinkel = 20, schraegungswinkel=10*dir);

}


circleOf(num=2, r=shift)rotate([0,90,0])
{
    wall();
    translate([0,0,-1])color("cyan")tube(do=12, di=8, h=1);
    translate([0,0,-1])mirror([0,0,1]) zahnrad();
}


/**


rotate([90,0,0])translate([0,0,shift]){
    wall();
    translate([0,0,-1])color("cyan")tube(do=12, di=8, h=1);
    translate([0,0,-1])mirror([0,0,1]) zahnrad(pfeilrichtung=-1);
}

rotate([180,0,0])translate([0,0,shift]){
    wall();
    translate([0,0,-1])color("cyan")tube(do=12, di=8, h=1);
    translate([0,0,-1])mirror([0,0,1]) zahnrad(pfeilrichtung=1);
}

**/
module wall(size=[50,50,10]){
    difference(){
        translate([0,0,10/2])cube(size, center=true);
        
        translate([0,0,-eps]){
            cylinder(d=8.2, h=12);
            cylinder(d=22, h=7);
            cylinder(d=12, h=8);
        }
        
        translate([0,0,size[2]/2])circleOf(num=4, r=size[0]/2-5){
           rotate([90,0,0])cylinder(d=4.2, h=size[0]+2, center=true);
        }
        translate([0,0,-eps]) rectOf(x=size[0]/2-5, y=size[1]/2-5){
           cylinder(d=4.2, h=size[2]+2);
        }
    }
}


module zahnrad(pfeilrichtung=1){
    difference(){
        union(){
            pfeilkegelrad(modul=1, zahnzahl=28,    teilkegelwinkel=45, zahnbreite=5, bohrung=dSchraube, eingriffswinkel = 20, schraegungswinkel=10*pfeilrichtung);
            mirror([0,0,1])translate([0,0,0]) tube(di=4.3, do=8, length=lTube);
        }
        translate([0,0,0.8])cylinder($fn=6, d=swHead/cos(30)+0.4, h=3);
        
    }
}
/***
translate([0,0,-1]) color("cyan") tube(di=8.4, do=13, h=1);

// wall
!translate([0,0,-1])
difference(){
    translate([-20,-20,-10])color("green")cube([40,40,10]);
    translate([0,0,eps-7]){
        cylinder(d=22.1, h=7);
        translate([]0,0,-1)cylinder(d=13, h=8);
    }
}
**********/

/*
rotate([90,0,360/24/2])translate([0,0,-12])
pfeilkegelrad(modul=1, zahnzahl=24, teilkegelwinkel=45, zahnbreite=5, bohrung=6, eingriffswinkel = 20, schraegungswinkel=-10);

mirror([0,0,1]) translate([0,0,-12])
pfeilkegelrad(modul=1, zahnzahl=24, teilkegelwinkel=45, zahnbreite=5, bohrung=6, eingriffswinkel = 20, schraegungswinkel=-10);
*/