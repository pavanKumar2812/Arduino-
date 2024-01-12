function myFunction()
{
  console.log("button was clicked!");
  var xhr = new XMLHttpRequest();
  var url = "/ledstate";

  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("led-state").innerHTML = this.responseText;
    }
  };

  xhr.open("GET", url, true);
  xhr.send();
};
document.addEventListener('DOMContentLoaded', myFunction, false);