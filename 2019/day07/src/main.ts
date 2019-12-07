import * as readline from 'readline';

let rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

let input = 1;
function getInput(): number {
  return input;
}

let output = 0;
function setOutput(v: number) {
  console.log(v);
  output = v;
}

enum Instr {
  Add = 1,
  Multiply = 2,
  Input = 3,
  Output = 4,
  JumpIfTrue = 5,
  JumpIfFalse = 6,
  LessThan = 7,
  Equals = 8,
  Halt = 99
};
enum Mode { Position = 0, Immediate = 1 };
class DecodedInstr {
  public instr: Instr;
  public instrMode: Mode;
  public param1Mode: Mode;
  public param2Mode: Mode;
  public param3Mode: Mode;
}

function decodeInstr(n: number): DecodedInstr {
  let di = new DecodedInstr();
  let de = n % 100;
  let c = Math.floor(n / 100) % 10;
  let b = Math.floor(n / 1000) % 10;
  let a = Math.floor(n / 10000) % 10;
  di.param1Mode = (c === 0) ? Mode.Position : Mode.Immediate;
  di.param2Mode = (b === 0) ? Mode.Position : Mode.Immediate;
  di.param3Mode = (a === 0) ? Mode.Position : Mode.Immediate;

  let e = de % 10;
  let instrKey = Object.keys(Instr).find(key => Instr[key] === e || Instr[key] === de); 
  if(instrKey == null) {
    throw new Error(`Invalid instrcution ${de}`);
  }
  di.instr = Instr[instrKey];
  if (di.instr !== Instr.Halt) {
    let d = Math.floor(de / 10);
    di.instrMode = (d === 0) ? Mode.Position : Mode.Immediate;
  } else {
    di.instrMode = Mode.Position;
  }
  return di;
}

function getValue(arr: number[], offset: number, mode: Mode): number {
  if (mode === Mode.Position) {
    return arr[arr[offset]];
  } else if (mode === Mode.Immediate) {
    return arr[offset];
  }
  throw new Error(`unknown mode ${mode}`);
}



class Amplifier
{
  private arr : number[];
  private ip : number;
  private output : number[];
  private input : number[];
  private outputCallback : (v: number) => void;
  private maxOutputLength : number;

  constructor(arr : number[]) {
    this.arr = arr.slice();
    this.ip = 0;
    this.input = [];
    this.output = [];
    this.maxOutputLength = null
  }

  setMaxOutputLength(n : number) {
    this.maxOutputLength = n;
  }

  getLastOutput() : number {
    if (this.output.length === 0) {
      throw new Error("no output");
    }
    return this.output[this.output.length-1];
  }

  setOutputCallback(callback : (v: number) => void) {
    this.outputCallback = callback;
  }

  getInput() : number {
    if(this.input.length === 0) {
      throw new Error("No Input");
    }
    return this.input.shift();
  }

  pushInput(v : number) {
    this.input.push(v);
  }

  setOutput(v : number) {
    this.output.push(v);
    if(this.outputCallback != null) {
      this.outputCallback(v);
    }
  }

  private runStep() {
    return this.runStepInternal(this.arr,this.ip);
  }

  private runStepInternal(arr,ip) {
    let di = decodeInstr(arr[ip]);
    let opCode = di.instr;
  
    if (opCode === Instr.Add) {
      let v1 = getValue(arr, ip + 1, di.param1Mode);
      let v2 = getValue(arr, ip + 2, di.param2Mode);
      console.assert(di.param3Mode === Mode.Position);
      arr[arr[ip + 3]] = v1 + v2;
      return ip + 4;
    } else if (opCode === Instr.Multiply) {
      let v1 = getValue(arr, ip + 1, di.param1Mode);
      let v2 = getValue(arr, ip + 2, di.param2Mode);
      console.assert(di.param3Mode === Mode.Position);
      arr[arr[ip + 3]] = v1 * v2;
      return ip + 4;
    } else if (opCode === Instr.Input) {
      console.assert(di.param1Mode === Mode.Position);
      arr[arr[ip + 1]] = this.getInput();
      return ip + 2;
    } else if (opCode === Instr.Output) {
      let v1 = getValue(arr, ip + 1, di.param1Mode);
      this.setOutput(v1);
      return ip + 2;
    } else if (opCode === Instr.JumpIfTrue) {
      let v1 = getValue(arr, ip + 1, di.param1Mode);
      let v2 = getValue(arr, ip + 2, di.param2Mode);
      return v1 !== 0 ? v2 : ip + 3;
    } else if (opCode === Instr.JumpIfFalse) {
      let v1 = getValue(arr, ip + 1, di.param1Mode);
      let v2 = getValue(arr, ip + 2, di.param2Mode);
      return v1 === 0 ? v2 : ip + 3;
    } else if (opCode === Instr.LessThan) {
      let v1 = getValue(arr, ip + 1, di.param1Mode);
      let v2 = getValue(arr, ip + 2, di.param2Mode);
      console.assert(di.param3Mode === Mode.Position);
      arr[arr[ip + 3]] = v1 < v2 ? 1 : 0;
      return ip + 4;
    } else if (opCode === Instr.Equals) {
      let v1 = getValue(arr, ip + 1, di.param1Mode);
      let v2 = getValue(arr, ip + 2, di.param2Mode);
      console.assert(di.param3Mode === Mode.Position);
      arr[arr[ip + 3]] = v1 === v2 ? 1 : 0;
      return ip + 4;
    }
    else if (opCode === Instr.Halt) {
      return -1;
    }
    throw new Error(`Unknown opCode ${opCode} at ip ${ip}, ${arr}`);
  }

  isFinished() : boolean {
    if(this.ip === -1) return true;
    if(this.maxOutputLength != null) {
      return this.output.length >= this.maxOutputLength;
    }
    return false;
  }

  isPaused() : boolean {
    if(this.ip === -1) return false;
    let di = decodeInstr(this.arr[this.ip]);
    let opCode = di.instr;
    if(opCode === Instr.Input) {
      return this.input.length === 0;
    }
    return false;
  }

  runSimInternal() {
    while(!this.isFinished() && !this.isPaused()) {
      this.ip = this.runStep();
    }
  }
}

function partATryAll() : number {
  let phases = [0,1,2,3,4];
  let build = [];
  let used = [false, false, false, false, false];
  return partATryCombo(build,phases,used,0);
}

function partATryCombo(build: number[],phases : number[],used : boolean[],index : number) : number {
  if(index === phases.length) {
    return partA(build);
  }
  let maxNum = null;
  for(let i = 0;i < phases.length;++i) {
    if(used[i]) continue;
    build[index] = phases[i];
    used[i] = true;
    let thisNum = partATryCombo(build,phases,used,index+1);
    used[i] = false;
    if(maxNum != null) {
      maxNum = Math.max(maxNum,thisNum);
    } else {
      maxNum = thisNum;
    }
  }
  return maxNum;
}

function partA(phases : number[]) : number {
  let amps = [];

  for(let i = 0;i < 5;++i) {
    amps.push(new Amplifier(arr));
    amps[i].pushInput(phases[i]);
    amps[i].setMaxOutputLength(1);
  } 
  amps[0].pushInput(0);
  for(let i = 1;i < 5;++i) {
    amps[i-1].setOutputCallback((v:number) => {
      amps[i].pushInput(v);
    });
  }
  for(let i = 0;i < 5;++i) {
    amps[i].runSimInternal();
  }
  return amps[amps.length-1].getLastOutput();
}

function partBTryAll() : number {
  let phases = [5,6,7,8,9];
  let build = [];
  let used = [false, false, false, false, false];
  return partBTryCombo(build,phases,used,0);
}

function partBTryCombo(build: number[],phases : number[],used : boolean[],index : number) : number {
  if(index === phases.length) {
    return partB(build);
  }
  let maxNum = null;
  for(let i = 0;i < phases.length;++i) {
    if(used[i]) continue;
    build[index] = phases[i];
    used[i] = true;
    let thisNum = partBTryCombo(build,phases,used,index+1);
    used[i] = false;
    if(maxNum != null) {
      maxNum = Math.max(maxNum,thisNum);
    } else {
      maxNum = thisNum;
    }
  }
  return maxNum;
}

function partB(phases : number[]) : number {
  let amps = [];

  for(let i = 0;i < 5;++i) {
    amps.push(new Amplifier(arr));
    amps[i].pushInput(phases[i]);
  } 
  for(let i = 1;i < 5;++i) {
    amps[i-1].setOutputCallback((v:number) => {
      amps[i].pushInput(v);
    });
  }
  amps[amps.length-1].setOutputCallback((v:number) => {
    amps[0].pushInput(v);
  });

  amps[0].pushInput(0);

  for(;;) {
    if(amps.every(a => a.isFinished())) {
      return amps[amps.length-1].getLastOutput();
    }
    if(amps.every(a => a.isPaused() || a.isFinished())) {
      throw new Error("Deadlock detected!");
    }
    for(let i = 0;i < 5;++i) {
      amps[i].runSimInternal();
    }
  }
}

let arr = []
rl.on('line', (line: string) => {
  let instr = line.split(',').map(x => parseInt(x, 10));
  instr.forEach(i => arr.push(i));
});

rl.on('close', () => {
  console.log("Part A");
  console.log(partATryAll());
  console.log("Part B");
  console.log(partBTryAll());
  
});