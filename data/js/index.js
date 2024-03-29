var tank_limit = 0;
var flow_rate = 0;
window.alarm = {};
var device_id, mqtt_host, mqtt_port, mqtt_user, mqtt_pass;
let mqttclient = null;
let tries = 1;
let maxtries = 5;
let connected = false;

let conn_options = {};
conn_options.retain = false;
conn_options.rejectUnauthorized = false;
conn_options.clean = true; // set to false to receive QoS 1 and 2 messages while offline
conn_options.reconnectPeriod = 5000; // milliseconds, interval between two reconnections
conn_options.connectTimeout = 10 * 1000; // milliseconds, time to wait before a CONNACK is received
conn_options.protocolVersion = 4;



document.addEventListener("DOMContentLoaded",(event)=>{
    //console.log(mqtt);
	device_id = $("body").attr('data-device-id');
	mqtt_host = $("body").attr('data-mqtt-host');
	mqtt_port = $("body").attr('data-mqtt-port');
	mqtt_user = $("body").attr('data-mqtt-user');
	mqtt_pass = $("body").attr('data-mqtt-pass');
	conn_options.username = mqtt_user;
	conn_options.password = mqtt_pass;
	conn_options.clientId = device_id + "-monitor";


    if(!connected) connect();
    $('#optin').click((e)=>{
        $('.optin-container').attr('data-status','disabled');
    });

    $('.topnav #arrowLeft').click((e)=>{
        document.location.href = "/";
		console.log(e);
    });

    $('.topnav #settings').click((e)=>{
        document.location.href = "settings";
    });

	if(device_id)
		document.title = device_id + " monitor";
	else 
		document.title = "IoT device monitor";
	

    //alarm
    window.alarm = new Audio("media/alarm.mp3");
    setInterval(trigger_alarm,2000)
})





function connect() {
	if(connected) return;
	console.log("mqtt","connecting...");

	mqttclient = mqtt.connect(`ws://${mqtt_host}:1888`,conn_options);
	if (mqttclient) {
		mqttclient.on('connect', onConnect);
		mqttclient.on('reconnect', onReconnect);
		mqttclient.on('disconnect', onDisconnect);
		mqttclient.on('error', onError);
		mqttclient.on('end', onEnd);
		mqttclient.on('close', onClose);
		mqttclient.on('offline', onOffline);
		mqttclient.on('message', onMessage);
	}
}



function onConnect() {

	connected = true;
	console.log("mqtt","connected as " + conn_options.username);
	tries = 1;
	const topic = `/devices/${device_id}/#`;

    mqttclient.subscribe(topic, function(err) {
		if (!err) {
			console.log("mqtt","subscribed to topic " + topic);
		} else {
			console.log("mqtt",err);
		}
	});    

}

function onReconnect() {
		if(tries>maxtries) {
			mqttclient.end();
			tries=0;
			return;
		}
		console.log("mqtt","reconnecting... #" + tries);
		tries++;
}

function onDisconnect(packet) {
	connected = false;
	console.log("mqtt","disconnected", packet);
}

function onError(error) {
	connected = false;
	console.log("mqtt","error: ", error);
}

function onEnd() {
	connected = false;
	console.log("mqtt","connection ended");
	mqttclient.removeAllListeners();
}

function onClose() {
	connected = false;
	console.log("mqtt","closing connection");
}

function onOffline() {
	connected = false;
	console.log("mqtt","offline");
}

function onMessage(topic, msg) {
    try {
        let msgdata = JSON.parse(msg.toString());
        console.log(topic,msgdata);
        $('.card#water_volume .value').html(numeral(Number(msgdata.data.water_vol)*100).format('0') + '%');
        $('.card#water_volume').attr('data-status',msgdata.data.tank_limit);
        $('.card#waterflow .value').html(numeral(Number(msgdata.data.flow_rate)).format('0.00'));
        $('.card#watertemp .value').html(numeral(Number(msgdata.data.water_temp)).format('0.00'));
        $('.card#temp .value').html(numeral(Number(msgdata.data.temp)).format('0.00'));
        $('.card#hum .value').html(numeral(Number(msgdata.data.hum)).format('0.00'));
        $('.last_update .timestamp').html(moment().format('dddd') + ", " + moment().format('HH:mm:ss'));
        flow_rate = msgdata.data.flow_rate;
        tank_limit = msgdata.data.tank_limit;
        trigger_alarm();
    } catch (e) {
        console.log("ERROR: " + e);
    }
}

function trigger_alarm(){
    if(tank_limit && flow_rate){
        window.alarm.play();
    }else{
        window.alarm.pause();
    }
}