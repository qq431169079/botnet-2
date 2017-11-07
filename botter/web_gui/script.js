var content = document.getElementById("content");
setInterval(setView,1000);
function setView () {
	var area = document.getElementById("connected");
	var total = document.getElementsByTagName("td").length;
	var nb = document.getElementsByClassName("connected").length;
	area.innerHTML = nb+"/"+total;
	loadData();
}
function loadData () {
    var request = new XMLHttpRequest;
    request.onreadystatechange = function(){
      if(request.readyState === 4 && request.status === 200) {
          content.innerHTML = request.responseText;
      }
    };
    request.open('GET','getData.php');
    request.send();
}
loadData ();