declare module "*.csv" {
    const Condition: any;
    const Argument: any;
    const InstructionTable: any;
}

declare module 'raw-loader!*' {
    const contents: string
    export = contents
}
