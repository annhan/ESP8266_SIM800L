byte dtmf_sim=10;
const char form_method_get[] PROGMEM = "</p><form method='get' action='";
const char br_html[] PROGMEM = "><br /><br />";
const char space_html[] PROGMEM ="&nbsp;";
const char support_html[] PROGMEM =   "";
const char label_html[] PROGMEM ="<li><label for=";
//const char label1_html[] PROGMEM ="<label for=";
const char _p_html[] PROGMEM ="</p>";
const char p_html[] PROGMEM ="<p>";
const char title_html[] PROGMEM ="</title></head><body>";
const char end_html[] PROGMEM = "</body></html>";
const char send_html[] PROGMEM ="text/html";
const char legend_style[] PROGMEM ="<legend style =\"color: red;  font-size : 150%;align: Center\">";
const char legend_end[] PROGMEM ="</legend>";
const char _fieldset[] PROGMEM ="</fieldset>";
const char fieldset[] PROGMEM ="<fieldset>";
const char _bodyhtml[] PROGMEM ="</body></html>";
const char legendhref[] PROGMEM ="<legend><a href='/";
const char information[] PROGMEM = "<li>Phạm An Nhàn 0363407090";
const char header[] PROGMEM = "<!DOCTYPE HTML>\r\n<html><head>"
                              "<style>"
                              "input {"
  "padding: 5px 0;"
  "font-size: 1.5em;"
  "font-family: inherit;"
  "width: 150px;"
"}"


                              
".weekDays-selector input {"
  "display: none!important;"
"}"

".weekDays-selector input[type=checkbox] + label {"
  "display: inline-block;"
  "border-radius: 6px;"
  "background: #dddddd;"
  "height: 40px;"
  "width: 30px;"
  "margin-right: 3px;"
  "line-height: 40px;"
  "text-align: center;"
  //cursor: pointer;
"}"

".weekDays-selector input[type=checkbox]:checked + label {"
  "background: #2AD705;"
  "color: #ffffff;"
"}"

  ".dropbtn {"
    "height: 50px;width: 150px;"
    "background-color: #4CAF50;"
    "color: white;"
    "padding: 10px;"
    "font-size: 20px;"  
"}"
                              
"nav {"
    "float: left;"
    "margin-right:30px;"
"}"                              
"legend {"
"background:#abedab;"
  "border:1px solid #000;"
"}"
"fieldset {"
  "background:#abedab;"
"}"

"body {background-color:#c9cfc9;}"
"h1 {"
    "font-size: 2em;"
    "text-align: center;"
"}"
"form .row {"
  "padding: 7px 8px;"
  "margin-bottom: 7px;"
"}"
"form .row:hover {"
  "background: #f1f7fa;"
"}"
"form label {"
  "display: inline-block;"
  "font-weight: bold;"
  "width: 200px;"
  "color: #464646;"
"}"

"form .req { color: #ca5354; }"

"form input:focus, form textarea:focus { outline: none; }"

"form .txt {"
  "display: inline-block;"
  "padding: 8px 9px;"
  "padding-right: 30px;"
  "width: 340px;"
  "transition: all 0.3s linear;"
"}"


"form .txt:focus, form .txtarea:focus {"
  "width: 400px;"
"}"

"form .txt:valid {"
  "background-color: #deecda;"
  "border: 1px solid #95bc7d;"
"}"

"#submitbtn {"
  "height: 50px;"
  "width: 150px;"
  "color: #0a528f;"
  "background-color: #4581e5;"
"}"
"#submitbtn:hover, #submitbtn:focus {"
  "box-shadow: 0 0 15px rgba(70, 100, 200, 0.9);"
"}"                  
                              "</style>"
                              "<meta charset='UTF-8'/>";


// "<link rel=\"shortcut icon\" href=\"data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAEAAAAAAAAAAAAAAAEAAAAAAADLy8sAHYsbACCNGADK28UADIEHAHOubwAShQoA//7/AObz6ADn8+gA6fPoAOzz6ADGxsYAXa5eAL7gugAghxAAtbyyABFmCAD09vEAN5ctADuPMAA4ly0AxuG9AKvXqQA2lzYAJ4cZADuXNgAikCIAKIwoAHS1cwDs++YAAHgAAKrKpwAAewAAj8WQACCHEQDa6tgAtbyzAMTcwQA4jzEAOpEuAFCkSwAYZgkAa7JiAIK9ggBAmDoAQJY9AA6FDADQ5NAApMalAC2PKQCozagA9PXwAPT48AAvbScAF2YKAGywZgBtsGYA4+3iAMviywA/mDsARpgyAM/jzgBkkGEA0eXLAC6KJAA2dDMAotCgAEWbRAA5iiQAv9+6AKmr3AAQZQgA8vXxAPn48QA+mDMAsdayAJ7DngChxKEANnQ0ACiRKABlkGIAq7iqAAB3AAAAegAAjsSQAHm5eQDKycoAw9vBAGGxYgB/mqMAhJesAPr6+AC21qoA/v/+AJqamgBygMIASpxAAMjIyADv8PYAYLBgANrr1gCUx5EAZ61gAGitYADQy9EAKGpOACOPJAD7+/wAJI8kAPv9+QDo8OUA/v//AI24iQD///8AWaxVAC+MJABerFUAXaleAF6pXgB5tHUA2uvXAPX16wDPys8AxOLGANHL0gAnhB8Agr1+AJqh1AAOfAgAzOHMAIe+gQCLvoEA6O7pAHK0bQCrt6oAMI4iAC6QKAAtlCsAS5s/AF+rXAB4t3YAwdrBAMrIygDv+e8A3O3eAM/K0ADf4uoAdJdxAHWXcQAniBoAbqtuAP3+9QAMhgwAoc6fAHO0bgAvjiMAFYYMAI28jgBGn0MAyMfIAGOrXQB4uHoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAyPe4c/NhEqT5QlaaAAX3JyXiITc5oWDmYpS11ycgByXIw8ZXJycnJycnxFg3JiclZESXJycnJycnJyOkEXfSScCXIvG1BtbW1rnYVAPRB4m3JyGgRxMU1OnoFycimVLlhycllTjXJycpAccnJmQpYLcnJkHx8zcnIeAnJyRjcPenJyDVNUBnJjgGBHchZIFDAHIAEoHWxyk1tqWnJDNjmDcm+KdCxycm4FK5hydVE7MhJyl34hGAeff3JyeRVScndoB3JyjpkfTHJyNC1VknIKYWcScnKRA3JyCIuhcFdycgl2iYQ+SjUmhoiiXHIMcnJycoI4JyMZLngkcnJyDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=\"/><meta charset='UTF-8' http-equiv=\"refresh\" content=\"60\" />";



const char begin_title[] PROGMEM ="<title>";


const char status_html_ok[] PROGMEM ="OK";

/*"border-bottom: 1px solid #BBBBBB;"
    "font-size: 2em;"
    "text-align: center;"
    "text-shadow: 0 2px #FFFFFF, 1px 3px #666666;"
"}"


"fieldset {"
  "margin-top:10px;"
  "background:#ffffcc;"
"}"

"legend {"
  "background:#ffffac;"


*/
