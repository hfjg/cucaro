use <hg_zahnradlib.scad>
use <baConEl.scad>

zh = 6;
z1 = 48;
z2 = 112;
r = (z1+z2)/2;
screwDia = 3.2;

echo ("r = ", r);
echo ("gear ratio = ", z2/z1);

module antrieb(){
    color("green")translate([r,0,0])pfeilrad(modul=1, zahnzahl=z1, hoehe=zh, bohrung=4.2, eingriffswinkel = 20, schraegungswinkel = -15);
}


module abtrieb(){
   
    difference(){
        union(){
            pfeilrad(modul=1, zahnzahl=z2, hoehe=zh, 
            bohrung=z2-32, eingriffswinkel = 20,
            schraegungswinkel = 15);
        
        // hub
            tube(di=10, wall=2, length=5);
        
        // spokes (break away after monting gearwheel)
            spokeWidth = 2;
            spokeHeight = 2;
            circleOf(r=6, num=6, startAngle=30)
                translate([0,-spokeWidth/2,0])cube([z2/2-10,spokeWidth,spokeHeight]);
            circleOf(r=z2/2-8, num=6)
               translate([0,0,zh-0.05])tube(di=screwDia, do=6, length=1);
        }
        
        // mount holes
        circleOf(r=z2/2-8, num=6){
           cylinder(d=screwDia, h=20, center=true);
           translate([0,0,-0.05])cylinder(d1=5.8, d2=screwDia);
        }
    }

}

wheelWidth = 32;
translate([0,0,-(wheelWidth/2+1+zh)]){
    antrieb();
    !abtrieb();
}
%axis();
%wheel();

module axis(){cylinder(d=8, h=60, center=true);}
module wheel(){tube(do=120, di=8, length=32, center=true);}
