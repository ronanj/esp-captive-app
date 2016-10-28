
var d3 = require("d3-selection");
import {ctrl} from "./ctrl"
 
export class Led
{
	color;
	time;
	rgb;

	constructor(div) {

		div = div
			.append("center")
			.attr("margin-top","20px")
		div.append("div")
			.attr("class","title")
			.html("Gizwits Led Settings")

		this.color = div
			.append("div")
			.style("width","80px")
			.style("height","80px")
			.style("background-color","white")
			.style("border","1px solid #eee")

		this.time = div
			.append("pre")
			.style("font-size","80%")
			.html("-");

		this.rgb = div
			.append("div")
			.selectAll("div")
			.data(["R","G","B"])
			.enter()
			.append("div")

		this.rgb
			.each((n,x,y)=>{
				let o = d3.select(y[x]);
				o.append("span")
					.style("font-size","180%")
					.style("color","#888")
					.html(n+" ");
				o.append("input")
					.attr("type","range")
					.attr("min",0)
					.attr("max",255)
					.attr("id","input-"+n)
					.style("width","80%")
					.style("margin","30px 0")
					.on("input",function(){
						let v = +this.value;
						ctrl.request("led:"+n.toLowerCase()+":"+v);
					});
			});



	}

	start()
	{
		ctrl.request("led:");
	}

	stop() {
	}

	update(val) {

		this.color.style("background-color","#"+val);

		let color = parseInt(val, 16);
		this.rgb
			.each((n,x,y)=>{
				let o = d3.select(y[x]);
				let v = (color>>(16-x*8))&0xff;
				o.select("input").property("value",v);
			});

		this.time.html("0x"+val);
	}
}