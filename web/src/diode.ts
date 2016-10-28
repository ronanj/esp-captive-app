
import {ctrl} from "./ctrl"

function pad(pad, str, padLeft=true) {
  if (typeof str === 'undefined') 
    return pad;
  if (padLeft) {
    return (pad + str).slice(-pad.length);
  } else {
    return (str + pad).substring(0, pad.length);
  }
}

export class Diode
{
	requests;
	time;
	val;

	constructor(div) {

		div = div
			.append("center")
			.attr("margin-top","20px")
		div.append("div")
			.attr("class","title")
			.html("Diode Readings")

		this.val = div
			.append("div")
			.style("font-size","500%")
			.html("-");

		this.time = div
			.append("pre")
			.style("font-size","80%")
			.html("-");
	}

	start()
	{
		this.requests =true;
		ctrl.request("diode:");
		console.log("start");

	}

	stop() {
		this.requests = false;
		console.log("stop");
	}

	update(val) {
		if (this.requests) {
			this.val.html(val);
			let d = new Date();


			let t = pad('00',d.getHours())+":"+pad('00',d.getMinutes())+":"+pad('00',d.getSeconds())+" "+pad('0000',d.getMilliseconds());
			this.time.html(t);

			setTimeout(()=>ctrl.request("diode:"),20);
		}
	}
}

