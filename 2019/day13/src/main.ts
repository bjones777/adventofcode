import * as readline from 'readline';
import { SSL_OP_SSLEAY_080_CLIENT_DH_BUG } from 'constants';

let rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

enum Instr {
  Add = 1,
  Multiply = 2,
  Input = 3,
  Output = 4,
  JumpIfTrue = 5,
  JumpIfFalse = 6,
  LessThan = 7,
  Equals = 8,
  AdjustRelativeBase = 9,
  Halt = 99
};

enum Mode { Position = 0, Immediate = 1, Relative = 2 };
class DecodedInstr {
  public instr: Instr;
  public instrMode: Mode;
  public param1Mode: Mode;
  public param2Mode: Mode;
  public param3Mode: Mode;
}

function decodeInstr(n: bigint): DecodedInstr {
  if(typeof n !== 'bigint') {
    console.log("here");
  }
  let di = new DecodedInstr();
  let de = n % 100n;
  let c = (n / 100n) % 10n;
  let b = (n / 1000n) % 10n;
  let a = (n / 10000n) % 10n;

  di.param1Mode = Mode[Object.keys(Mode).find(key => Mode[key] == c)];
  di.param2Mode = Mode[Object.keys(Mode).find(key => Mode[key] == b)];
  di.param3Mode = Mode[Object.keys(Mode).find(key => Mode[key] == a)];

  let instrKey : string = null;
  if(de != BigInt(Instr.Halt)) {
    let e = de % 10n;
    instrKey = Object.keys(Instr).find(key => Instr[key] == e); 
  }
  else {
    instrKey = Object.keys(Instr).find(key => Instr[key] == de); 
  }
  
  if(instrKey == null) {
    throw new Error(`Invalid instruction ${de}`);
  }
  di.instr = Instr[instrKey];
  if (di.instr !== Instr.Halt) {
    let d = de / 10n;
    di.instrMode = (d == 0n) ? Mode.Position : Mode.Immediate;
  } else {
    di.instrMode = Mode.Position;
  }
  return di;
}

class Simulator
{
  private arr : bigint[];
  private ip : number;
  private output : bigint[];
  private input : bigint[];
  private outputCallback : (v: bigint) => void;
  private inputCallback : () => bigint;
  private maxOutputLength : number;
  private relativeBase : number;

  constructor(arr : bigint[]) {
    this.arr = arr.slice();
    this.ip = 0;
    this.input = [];
    this.output = [];
    this.maxOutputLength = null;
    this.relativeBase = 0;
  }

  readMem(address : number) : bigint {
    if(this.arr[address] == null) {
      this.arr[address] = 0n;
    }
    return this.arr[address];
  }

  readMem2(address : number) : number {
    if(this.arr[address] == null) {
      this.arr[address] = 0n;
    }
    return Number(this.arr[address].toString());
  }

  writeMem(address : number,value : bigint,mode: Mode) : void {
    if(mode === Mode.Position) {
      this.arr[address] = value;
    } else if(mode === Mode.Relative) {
      this.arr[this.relativeBase + address] = value;
    }
    else {
      throw new Error(`Unhandled mode ${mode}`);
    }
  }

  setMaxOutputLength(n : number) {
    this.maxOutputLength = n;
  }

  getLastOutput() : bigint {
    if (this.output.length === 0) {
      throw new Error("no output");
    }
    return this.output[this.output.length-1];
  }

  setOutputCallback(callback : (v: bigint) => void) {
    this.outputCallback = callback;
  }

  setInputCallback(callback : () => bigint) {
    this.inputCallback = callback;
  }

  getInput() : bigint {
    if(this.input.length === 0 && this.inputCallback != null) {
      let newValue = this.inputCallback();
      if(newValue != null) {
        this.input.push(newValue);
      }
    }
    if(this.input.length === 0) {
      throw new Error("No Input");
    }
    return this.input.shift();
  }

  pushInput(v : bigint) {
    this.input.push(v);
  }

  setOutput(v : bigint) {
    this.output.push(v);
    if(this.outputCallback != null) {
      this.outputCallback(v);
    }
  }

  private runStep() {
    return this.runStepInternal();
  }

  private runStepInternal() {
    let arr = this.arr;
    let ip = this.ip;
    let di = decodeInstr(this.readMem(ip));
    let opCode = di.instr;
  
    if (opCode === Instr.Add) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      let v2 = this.getValue(ip + 2, di.param2Mode);
      this.writeMem(this.readMem2(ip + 3),v1+v2,di.param3Mode);
      return ip + 4;
    } else if (opCode === Instr.Multiply) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      let v2 = this.getValue(ip + 2, di.param2Mode);
      this.writeMem(this.readMem2(ip + 3),v1*v2,di.param3Mode);
      return ip + 4;
    } else if (opCode === Instr.Input) {
      this.writeMem(this.readMem2(ip+1),this.getInput(),di.param1Mode);
      return ip + 2;
    } else if (opCode === Instr.Output) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      this.setOutput(v1);
      return ip + 2;
    } else if (opCode === Instr.JumpIfTrue) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      let v2 = this.getValue(ip + 2, di.param2Mode);
      return v1 != 0n ? Number(v2.toString()) : ip + 3;
    } else if (opCode === Instr.JumpIfFalse) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      let v2 = this.getValue(ip + 2, di.param2Mode);
      return v1 == 0n ? Number(v2.toString()) : ip + 3;
    } else if (opCode === Instr.LessThan) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      let v2 = this.getValue(ip + 2, di.param2Mode);
      this.writeMem(this.readMem2(ip + 3),v1 < v2 ? 1n : 0n,di.param3Mode);
      return ip + 4;
    } else if (opCode === Instr.Equals) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      let v2 = this.getValue(ip + 2, di.param2Mode);
      this.writeMem(this.readMem2(ip + 3),v1 === v2 ? 1n : 0n,di.param3Mode);
      return ip + 4;
    }
    else if(opCode === Instr.AdjustRelativeBase) {
      let v1 = this.getValue(ip + 1, di.param1Mode);
      let offset = Number(v1.toString());
      if(offset.toString() !== v1.toString()) {
        console.assert(false);
      }
      this.relativeBase += offset;
      return ip + 2;
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
    if(this.inputCallback != null) return false;
    let di = decodeInstr(this.readMem(this.ip));
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

  getValue(offset: number, mode: Mode): bigint {
    if (mode === Mode.Position) {
      let index = this.readMem2(offset);
      return this.readMem(index);
    } else if (mode === Mode.Immediate) {
      return this.readMem(offset);
    } else if(mode === Mode.Relative) {
      let newOffset = this.relativeBase + this.readMem2(offset);
      return this.readMem(newOffset);
    }
    throw new Error(`unknown mode ${mode}`);
  }
}

class Point {
  public x : number;
  public y : number;

  constructor(x:number = 0, y:number = 0) {
    this.x = x;
    this.y = y;
  }

  getKey() : string {
      return `${this.x.toString()},${this.y.toString()}`; 
  }
}



let arr = []
rl.on('line', (line: string) => {
  let instr = line.split(',').map(x => BigInt(x));
  instr.forEach(i => arr.push(i));
});

function partA() {
  let s = new Simulator(arr);
  let robotPoint = new Point();
  let panels = new Map<string,number>();
  let phase = 0;
  s.setOutputCallback((v:bigint) => {
    if(phase === 0) {
      robotPoint.x = parseInt(v.toString(),10);
    } else if(phase === 1) {
      robotPoint.y = parseInt(v.toString(),10);
    } else {
      panels.set(robotPoint.getKey(),parseInt(v.toString(),10));
    }
    ++phase;
    phase %= 3;
  });
  s.runSimInternal();
  let numBlockTiles = Array.from(panels.values()).reduce((pv,v) => pv + (v === 2 ? 1 : 0),0);
  console.log(numBlockTiles);
}

function partB() {
  let newArr = arr.slice();
  newArr[0] = 2n;
  let s = new Simulator(newArr);
  let robotPoint = new Point();
  let panels = new Map<string,number>();
  let phase = 0;
  let score = 0;
  s.setInputCallback(() => {
    let tiles = Array.from(panels.entries());
    let ball = tiles.find(x => x[1] === 4);
    let paddle = tiles.find(x => x[1] === 3);
    if(ball == null || paddle == null) {
      return 0n;
    }
    let ballX = parseInt(ball[0].split(',')[0],10);
    let paddleX = parseInt(paddle[0].split(',')[0],10);
    if(ballX < paddleX) {
      return -1n;
    } else if(ballX > paddleX) {
      return 1n;
    }
    return 0n;
  });
  s.setOutputCallback((v:bigint) => {
    if(phase === 0) {
      robotPoint.x = parseInt(v.toString(),10);
    } else if(phase === 1) {
      robotPoint.y = parseInt(v.toString(),10);
    } else {
      if(robotPoint.x === -1 && robotPoint.y === 0) {
          score = parseInt(v.toString(),10);
      } else {
        panels.set(robotPoint.getKey(),parseInt(v.toString(),10));
      }
    }
    ++phase;
    phase %= 3;
  });
  s.runSimInternal();
  let numBlockTiles = Array.from(panels.values()).reduce((pv,v) => pv + (v === 2 ? 1 : 0),0);
  console.log(numBlockTiles);
  console.log(score);
}

rl.on('close', () => {
  console.log("Part A");
  partA();

  console.log("Part B");
  partB();
});