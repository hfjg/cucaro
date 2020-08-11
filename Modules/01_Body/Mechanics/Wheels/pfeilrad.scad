use <hg_zahnradlib.scad>
use <baConEl.scad>

zBig = 106;
zSmall = 24;
zModule = 1;
axisDistance = (zBig+zSmall)/2/zModule;
echo("Axis distance = ", axisDistance);

module bigWheel(){
    difference(){
        union(){
            pfeilrad(modul=zModule, zahnzahl=zBig, hoehe=5, bohrung=84, eingriffswinkel = 20, schraegungswinkel = 20);
            tube(do=28.4, wall=2.4, length=6);
            circleOf(num=6, r=28/2, startAngle=30) translate([0,-0.5,0])cube([40,1.4,1.4]);
        }
        
        circleOf(num=6, r=94/2){
            cylinder(d=3.2, h=20, center=true);
            translate([0,0,-0.05])cylinder(d1=5.6, d2=3.2, h=2.2);
           
        }
         circleOf(num=6,r=24)translate([-15,-4,3])cube([8,8,10]);
    }
}

module smallWheel(){
    pfeilrad(modul=zModule, zahnzahl=zSmall, hoehe=4, bohrung=3, eingriffswinkel = 20, schraegungswinkel = -20);
}

bigWheel();

translate([(zBig+zSmall)/2/zModule,0,0.50])rotate([0,0,360/zSmall/2])smallWheel();
