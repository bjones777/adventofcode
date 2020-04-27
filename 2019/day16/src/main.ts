import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

let pattern = [1, 0, -1, 0];

function calcDigitSlow(input : number[],input2: number[], index : number) : number {
  let v = 0;
  for(let i = index;i < input.length; ++i) {
    let pi = Math.floor((i - index)/(index+1)) % pattern.length; 
    let pv = pattern[pi];
    v += pv * input[i];
  }
  if(v < 0) {
    v = -v;
  }
  v = v % 10;
  return v;
}

function calcDigit(input : number[],input2: number[], index : number) : number {
  let v = 0;
  for(let i = index;i < input.length; ++i) {
    let pi = Math.floor((i - index)/(index+1)) % pattern.length; 
    let pv = pattern[pi];
    if(pv === 0) {
      i += index;
    } 
    else if(pv === 1) {
      v += input2[Math.min(input2.length-1,i + index + 1)] - input2[i];
      i += index;
    }
    else if(pv === -1) {
      v -= input2[Math.min(input2.length-1,i + index + 1)] - input2[i];
      i += index;
    }
    else {
      v += pv * input[i];
    }
  }
  if(v < 0) {
    v = -v;
  }
  v = v % 10;
  return v;
}

  function FFT(input : number[]) : number[] {
    let sumTable = [0];
    for(let i = 0;i < input.length;++i) {
      sumTable.push(sumTable[sumTable.length-1] + input[i]);
    }
    let retVal = [];
    for(let i = 0;i < input.length;++i) {
      retVal.push(calcDigit(input,sumTable, i));
    }
    return retVal;
  }

  let i = "12345678";
  console.log(FFT(convertSignalToInput(i)));

  function convertSignalToInput(signal: string) : number[] {
    let input = [];
    for(let i = 0;i < signal.length;++i) {
      input.push(Number(signal[i]));
    }
    return input;
  }

  function partA(signal : string) {
    let input = convertSignalToInput(signal);
    
    for(let i = 0;i < 100;++i) {
      input = FFT(input);
    }
    console.log(input.map(x => x.toString()).join('').substr(0,8));
  }

  function partB(signal : string) {
    let offset = parseInt(signal.substr(0,7),10);
    let input = convertSignalToInput(signal);
    let realInput = [];
    for(let i = 0;i < input.length * 10000;++i) {
      realInput.push(input[i % input.length]);
    } 
    input = realInput;
    
    for(let i = 0;i < 100;++i) {
      input = FFT(input);
    }
    signal = input.map(x => x.toString()).join('');
    console.log(signal.substr(offset,8));
  }

rl.on('line',(line : string) => {
    let signal = line;
    console.log(signal.length);
    partA(signal);
    partB(signal);
}); 