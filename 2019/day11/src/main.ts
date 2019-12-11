import * as readline from 'readline';

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

enum Direction {
  N, S, E, W
};

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

enum TurnDir {
  Left = 0,
  Right = 1,
}

function turn(td : TurnDir,id:Direction) : Direction {
  if(td === TurnDir.Left) {
    if(id === Direction.N) {
      return Direction.W;
    } else if(id === Direction.W) {
      return Direction.S;
    } else if(id === Direction.S) {
      return Direction.E;
    } else {
      console.assert(id === Direction.E);
      return Direction.N;
    }
  } else {
    console.assert(td === TurnDir.Right);
    if(id === Direction.N) {
      return Direction.E;
    } else if(id === Direction.E) {
      return Direction.S;
    } else if(id === Direction.S) {
      return Direction.W;
    } else {
      console.assert(id === Direction.W);
      return Direction.N;
    }
  }
}

function move(p : Point,d : Direction) {
  if(d === Direction.N) {
    p.y -= 1;
  } else if(d === Direction.S) {
    p.y += 1;
  } else if(d === Direction.E) {
    p.x += 1;
  } else {
    console.assert(d === Direction.W);
    p.x -= 1;
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
  let dir = Direction.N;
  let painted = new Set<string>();
  s.setInputCallback(() : bigint => {
    let key = robotPoint.getKey();
    if(!panels.has(key)) {
      panels.set(key,0);
    }
    return BigInt(panels.get(key));
  });
  let phase = 0;
  s.setOutputCallback((v:bigint) => {
    if(phase === 0) {
      let key = robotPoint.getKey();
      panels.set(key,parseInt(v.toString(),10));
      painted.add(key);
    } else {
      let t = parseInt(v.toString(),10);
      dir = turn(t,dir);
      move(robotPoint,dir);
    }
    ++phase;
    phase %= 2;
  });
  s.runSimInternal();
  console.log(painted.size);
}

function partB() {
  let s = new Simulator(arr);
  let robotPoint = new Point();
  let panels = new Map<string,number>();
  let dir = Direction.N;
  let key = robotPoint.getKey();
  panels.set(key,1);
  let minX = 0;
  let maxX = 0;
  let minY = 0;
  let maxY = 0;
  s.setInputCallback(() : bigint => {
    let key = robotPoint.getKey();
    if(!panels.has(key)) {
      panels.set(key,0);
    }
    return BigInt(panels.get(key));
  });
  let phase = 0;
  s.setOutputCallback((v:bigint) => {
    if(phase === 0) {
      minX = Math.min(minX,robotPoint.x);
      maxX = Math.max(maxX,robotPoint.x);
      minY = Math.min(minY,robotPoint.y);
      maxY = Math.max(maxY,robotPoint.y);
      let key = robotPoint.getKey();
      panels.set(key,parseInt(v.toString(),10));
    } else {
      let t = parseInt(v.toString(),10);
      dir = turn(t,dir);
      move(robotPoint,dir);
    }
    ++phase;
    phase %= 2;
  });
  s.runSimInternal();
  for(let y = minY;y <= maxY;++y) {
    robotPoint.y = y;
    let line = "";
    for(let x = minX;x <= maxX;++x) {
      robotPoint.x = x;
      let key = robotPoint.getKey();
      if(panels.has(key) && panels.get(key)===1) {
        line += 'X';
      }
      else {
        line += ' ';
      }
    }
    console.log(line);
  }
}

rl.on('close', () => {
  console.log("Part A");
  partA();

  console.log("Part B");
  partB();
});