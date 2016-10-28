

var d3 = require("d3-selection");
import {ctrl} from "./ctrl";
import {Led} from "./led"
import {Diode} from "./diode"

export class UI
{
	console:string = "";
	currentTab = 1;
	connectBtn;
	consoleDiv;

	headers;
	contents;
	
	diode: Diode;
	led: Led;

	init(root)
	{

		let tabs = [0,1,2];

		let names = ["Console","Diode","Lights"];

		this.headers = d3.select(root)
			.append("div")
			.attr("class","tabs")
			.selectAll("div")
			.data(tabs)
			.enter()
			.append("div")
			.attr("class",n=>n==0?"active":"")
			.attr("id",n=>"h"+n)
			.html(n=>names[n])
			.on("click",n=>{
				this.goto(n);
			});

		this.contents = d3.select(root)
			.append("div")
			.attr("class","contents")
			.selectAll("div")
			.data(tabs)
			.enter()
			.append("div")
			.attr("class",n=>"content "+(n==0?"active":""))
			.attr("id",n=>"c"+n);

		let mainContent = d3.select("#c0");
		this.connectBtn = mainContent
			.append("div")
			.attr("class","btn")
			.html("Connect") 
			.on("click",()=>{
				ctrl.connect();
			});

		this.consoleDiv = mainContent
			.append("div")
			.style("display","block")
			.html("---");

		this.diode = new Diode(d3.select("#c1"));
		this.led = new Led(d3.select("#c2"));

		var element = document.createElement('style');
		let css = require("./style.css")
		element.type = "text/css";
		element.innerHTML = css
		document.body.appendChild(element);

		this.log("Init");

		setTimeout(()=>{
			ctrl.connect();
		},250);

	}

	connectState(s) {
		this.connectBtn.html(s);
		this.connectBtn.style("display",(s=="Disconnect")?"none":"block");
		if (s=="Disconnect") this.goto(2);

	}

	log(s) {
		var t = new Date().toTimeString();
		this.console += `<small>[${t}]</small> ${s}<br>`;
		this.consoleDiv.html(this.console);

	}

	goto(id) {
		this.headers.attr("class","");
		this.contents.attr("class","content");

		d3.select("#h"+id).attr("class","active");
		d3.select("#c"+id).attr("class","active");
		this.currentTab = id;

		console.log("goto",id);

		if (id==1) {
			this.diode.start();
		} else {
			this.diode.stop();
		}

		if (id==2) {
			this.led.start();
		}
	}

	next() {
		this.currentTab ++;
		if (this.currentTab>=3) this.currentTab=0;
		this.goto(this.currentTab);
	}

}

export let ui = new UI();
