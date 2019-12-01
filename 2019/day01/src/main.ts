import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

function partA(mass) {
  return Math.floor(mass / 3) - 2;
}

function partB(mass) {
  let amount = partA(mass);
  if(amount <= 0) {
    return 0; 
  }
  return amount + partB(amount);
}

var arr = [];
rl.on('line',(line : string) => {
    let mass = parseInt(line,10);
    arr.push(mass);
}); 

rl.on('close',() => {
  let ansA = arr.reduce((prev, mass) => prev + partA(mass),0);
  console.log(ansA);
  
  let ansB = arr.reduce((prev, mass) => prev + partB(mass),0);
  console.log(ansB); 
});