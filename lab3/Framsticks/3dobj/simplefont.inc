function makeVectorText(txt,x,y,z,scalex,scaley,spacing)
{
var i;
var xyz=[];

var defs=" =. A=14863/46. B=786317/46. C=9713. D=786217. E=9713/45. F=971/45. G=984236. H=71/93/46. I=82. J=89314. K=71/943. L=713. M=17593. N=1739. O=17931. P=17964. Q=17931/35. R=179643. S=974631. T=79/82. U=7139. V=729. W=71539. X=73/19. Y=759/52. Z=7913. 0=17931. 1=82. 2=79613. 3=79531. 4=746/52. 5=974621. 6=9841364. 7=792. 8=79317/46. 9=1269746. .=52. ,=51.";
txt=String.toUpper(txt);
for(i=0;i<String.len(txt);i++)
	{
	var ch=String.substr(txt,i,1);
	var pp=String.indexOf(defs,ch+"=");
	if (pp>=0)
		{
		pp+=2;
		var prev="";
		while(pp<String.len(defs))
			{
			ch=String.substr(defs,pp,1);
			if (ch==".") break;
			if (ch=="/")
				{
				prev="";
				}
			else 
				{
				if (prev!="")
				{
				vectortxt_add_xyz(xyz,x,y,z,scalex,scaley,ch);
				vectortxt_add_xyz(xyz,x,y,z,scalex,scaley,prev);
				}
				prev=ch;
				}
			pp++;
			}
		x+=scalex+spacing;
		}
	}
return xyz;
}

function vectortxt_add_xyz(xyz,x,y,z,scalex,scaley,ch)
{
var c=String.code(ch);
var dx=0.5*scalex*((c-49)%3);
var dy=0.5*scaley*((c-49)/3);
xyz.add(x+dx);
xyz.add(y+dy);
xyz.add(z);
}
