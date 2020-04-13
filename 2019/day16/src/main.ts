import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

let pattern = [1n, 0n, -1n, 0n];

function calcDigit(input : string,index : number) : string {
  let v = 0n;
  for(let i = index;i < input.length; ++i) {
    let pi = Math.floor((i - index)/(index+1)) % pattern.length;  
    v += pattern[pi] * BigInt(input[i]);
  }
  if(v < 0) {
    v = -v;
  }
  v = v % 10n;
  return v.toString();
}

  function FFT(input : string) : string {
    let output = [];
    for(let i = 0;i < input.length;++i) {
      output.push(calcDigit(input,i));
    }
    return output.join('');
  }

  let i = "12345678";
  console.log(FFT(i));

  function partA(signal : string) {
    for(let i = 0;i < 100;++i) {
      signal = FFT(signal);
    }
    console.log(signal.substr(0,8));
  }

  function partB(signal : string) {
   let realSignal = signal;
    for(let i = 0;i < 10000;++i) {
      realSignal += signal;
    }
    for(let i = 0;i < 100;++i) {
      console.log(`${i}...`);
      realSignal = FFT(realSignal);
    }
    let offset = Number(signal.substr(0,7));
    console.log(realSignal.substr(offset,8));
  }

rl.on('line',(line : string) => {
    let signal = line;
    console.log(signal.length);
    partA(signal);
    partB(signal);

}); 