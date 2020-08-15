use <hg_zahnradlib.scad>
use <baConEl.scad>

module carrier(zz=60, drillDia=8.2, wx=4, pfeil=20){
    difference(){
        union(){
            //pfeilrad(modul=1, zahnzahl=zz, hoehe=6, bohrung=2, eingriffswinkel = 20, schraegungswinkel = pfeil);


  pfeilhohlrad(modul=1, zahnzahl=zz, hoehe=10, randbreite=5, eingriffswinkel = 20, schraegungswinkel = 0);
            
            cylinder(d=40, h=2);
            for (phi=[0:90:179]) rotate([0,0,phi])translate([-(zz-5)/2,-wx/2,-3])
                cube([zz-5,wx,3]);
        }
        cylinder(d=drillDia, h=20, center=true);
    }
}

r = 45;
zh = 6;
z1 = 12;
z2 = z1 + 2*r;

color("green")translate([r,0,0])pfeilrad(modul=1, zahnzahl=z1, hoehe=zh, bohrung=4, eingriffswinkel = 20, schraegungswinkel = -15);

!union(){
    difference(){
        translate([0,0,-2]){
            translate([0,0,-3])tube(di=80, do=114.333, length=3);
            tube(di=105, do=114.333, length=3);
            translate([0,0,-3])tube(di=8.2, do=14, length=3);
            
            circleOf(r=6, num=6, startAngle=30)translate([0,-4,-3])cube([45,8,3]);
        }
        circleOf(r=45, num=6){
           cylinder(d=3.2, h=20, center=true);
           translate([0,0,-3+0.05])cylinder(d2=5.8, d1=3.2);
        }
    }

    pfeilhohlrad(modul=1, zahnzahl=z2, hoehe=zh, randbreite=5, eingriffswinkel = 20, schraegungswinkel = 15);
}