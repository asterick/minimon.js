declare module "*.csv" {
    const Condition: {[key: string]: number};
    const Argument: {[key: string]: number};
    const InstructionTable: any;
}

declare module 'raw-loader!*' {
    const contents: string
    export = contents
}
