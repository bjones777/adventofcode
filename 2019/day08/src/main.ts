import * as readline from 'readline';

const width = 25;
const height = 6;

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

let data = "";

rl.on('line',(line : string) => {
    data += line; 
}); 

function countString(layer:string,toCount:string) : number {
  let numZeros = 0;
  for(let i = 0;i < layer.length;++i) {
    if(layer[i] === toCount) {
      numZeros += 1;
    }
  }
  return numZeros;
}

function partA() {
  let fewestZeroLayer = null;
  let numZeros = -1;
  let numLayers = Math.floor(data.length/(25*6));
  for(let l = 0;l < numLayers;++l) {
    let layer = data.substr(l*width*height,width*height);
    let cv = countString(layer,"0");
    if(fewestZeroLayer == null || cv < numZeros) {
      fewestZeroLayer = layer;
      numZeros = cv;
    }
  }
  console.log(countString(fewestZeroLayer,"1")*countString(fewestZeroLayer,"2"));  
}

function determineColor(layers : string[],x : number,y : number) : string {
  for(let i = 0;i < layers.length;++i) {
    let layer = layers[i];
    if(layer[y*width+x] === "2") continue;
    return layer[y*width+x] === "1" ? "1" : " "; 
  }
  throw new Error("Unknown color");
}

function partB() {
  let layers = [];
  let numLayers = Math.floor(data.length/(25*6));
  for(let l = 0;l < numLayers;++l) {
    let layer = data.substr(l*width*height,width*height);
    layers.push(layer);
  }
  
  for(let y = 0;y < height;++y) {
    let line = "";
    for(let x = 0;x < width;++x) {
      line += determineColor(layers,x,y);
    }
    console.log(line);
  }
}

rl.on('close',() => {
  partA();
  partB();
}); 