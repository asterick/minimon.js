interface UniformList {
	[key: string]: WebGLUniformLocation;
}

interface AttributeList {
	[key: string]: number;
}

interface Instruction {
    op: string;
    condition: number;
    args: number[];
}

declare module "*.csv" {
    const Condition: {[key: string]: number};
    const Argument: {[key: string]: number};
    const InstructionTable: (Instruction | Instruction[] | null)[];
}

declare module 'raw-loader!*' {
    const contents: string
    export = contents
}
