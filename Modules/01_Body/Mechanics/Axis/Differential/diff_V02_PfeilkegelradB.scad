use <hg_zahnradlib.scad>


eps=0.05;
$fn=36;


!diffZahnRadB(-1);


module diffZahnRadB(dir=1){
    difference(){
        cylinder(d1=26, d2=41, h=7);
        translate([0,0,-eps])cylinder(d=22.1, h=15);
    }
    translate([0,0,6.5])
    pfeilkegelrad(modul=1.5, zahnzahl=28,    teilkegelwinkel=45, zahnbreite=5, bohrung=22.1, eingriffswinkel = 20, schraegungswinkel=10*dir);

}
