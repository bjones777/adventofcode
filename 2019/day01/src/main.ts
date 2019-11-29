import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

rl.on('line',(line : string) => {
    let freqs = line.split(',');
    let res = freqs.reduce((sum, cv) => sum + parseInt(cv,10), 0);
    console.log(res);    
}); 