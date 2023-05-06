document.addEventListener("DOMContentLoaded",(event)=>{
    $('.topnav #arrowLeft').click((e)=>{
        document.location.href = "/";
    });
	if($(".topnav .device_id").html() != '%DEVICE_ID%')
		document.title = $(".topnav .device_id").html();
	else 
		document.title = "IoT device monitor";
	    
})