import * as React from 'react';
import * as ReactDOM from 'react-dom';

import * as Table from './s1c88.csv';
import './index.css';

async function render() {
    ReactDOM.render(<h2>Hello from React!</h2>, document.getElementById("container"));

    console.log(Table);
    const data = await fetch('/static/libminimon.wasm');
    console.log(await data.arrayBuffer());
}

render();
