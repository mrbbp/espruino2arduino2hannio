/*
  the code write text with a custom font in an ArrayBuffer (espruino example with the graphics lib)
  the text is shown in a small flipdot display (5x7) ou led matrix 
  The text slide from rigth to left by one unit (one dot) each 100ms
  send datas (arrayBuffer) to arduino via Serial

  There is a handshake to begin to send datas

  Espruino TX pin: B6
  Espruino RX pin: B7
  
  coded by mrbbp.com - January 2018
  from differents sources & discussion
*/

var compteur = 0;
var g;
var longT;

// your text
var text = "Le vent soulevait, le vent baissait, soulevait sa jupe en laine moche.";


Graphics.prototype.print = function() {
  for (var y=0;y<this.getHeight();y++)
  	console.log(new Uint8Array(this.buffer,this.getWidth()*y,this.getWidth()).toString());
};

function sendDatas() {
  if (compteur < -longT) {
    compteur = 7;
    console.log("reset compteur: ",compteur);
  }
  // creat arrayBuffer 5x7 8bits
  g = Graphics.createArrayBuffer(5,7,8);
  // select font
  g.setFont4x6();
  g.setColor(1);
  // draw text in the buffer
  g.drawString(text,compteur,-1);

  var datas = new Uint8Array(g.buffer);
  // reset position of the text when finished
  if (compteur === 0) {
    longT = g.stringWidth(text);
    console.log(compteur, longT);
  }
  // send datas
  Serial1.write(datas);
  // send a final Mark
  Serial1.print('>');
  //g.print(); // for debugging
  // toggle led on the espruino
  LED1.toggle();
  // change value to move text displayed
  compteur--;
}

function onInit() {
  // add custom font
  require("Font4x6").add(Graphics);
  // initiate a serial port
  Serial1.setup(19200, {tx:B6,rx:B7});
  // wainting for init of serial com from arduino before to send datas
  Serial1.on('data', function(e) {
    console.log("serial In:", e);
    // begin Mark, sent by arduino when serial.begin() initiated
    if (e == '@') {
      setInterval(sendDatas, 100);
      // remove this listener
      Serial1.removeAllListeners();
    }
  });
  // to avoid problem with the console.log when usb is not connected
  USB.setConsole();
}