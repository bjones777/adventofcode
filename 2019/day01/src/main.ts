import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

let partA = function(mass) {
  return Math.floor(mass / 3) - 2;
}

let partB = function(mass) {
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
    if(arr.length === 100) {
      let ans = arr.reduce((prev, mass) => prev + partA(mass),0);
      console.log(ans);    
    }
    if(arr.length === 100) {
      let ans = arr.reduce((prev, mass) => prev + partB(mass),0);
      console.log(ans);    
    }
}); 