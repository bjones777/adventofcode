import * as readline from 'readline';

let rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: false
  });

function runStep(arr, ip) {
  let opCode = arr[ip];
  if(opCode === 1) {
    let v1 = arr[arr[ip+1]];
    let v2 = arr[arr[ip+2]];
    arr[arr[ip+3]] = v1 + v2;
    return ip + 4;
  } else if(opCode === 2) {
    let v1 = arr[arr[ip+1]];
    let v2 = arr[arr[ip+2]];
    arr[arr[ip+3]] = v1 * v2;
    return ip + 4;
  }
  else if(opCode === 99) {
    return -1;
  }
  throw new Error(`Unknown opCode ${opCode} at ip ${ip}, ${arr}`);
}

function runSim(arr) {
  let ip = 0;
  for(;;) {
    ip = runStep(arr,ip);
    if(ip < 0) {
      return arr[0];
    }
  }
}

function partB(arr) {
  for(let noun = 0;noun <= 99;++noun) {
    for(let verb = 0;verb <= 99;++verb) {
      try {
        let partB = arr.slice();
        partB[1] = noun;
        partB[2] = verb;
        let ans = runSim(partB);
        if(ans === 19690720) {
          return 100 * noun + verb;
        }
      }
      catch(e) {
      }
    }
  }
}

let arr = []
rl.on('line',(line : string) => {
    let instr = line.split(',').map(x => parseInt(x, 10));
    instr.forEach(i => arr.push(i));
}); 

rl.on('close', () => {
  {
    let partA = arr.slice();
    partA[1] = 12;
    partA[2] = 2;
    let ans = runSim(partA);
    console.log(`part A: ${ans}`);
  }
  let ansB = partB(arr);
  console.log(`part B: ${ansB}`);
});