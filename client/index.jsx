import { render } from 'inferno';
import UI from  "./ui/index.jsx";

console.log(require("./system/bios"));

(async _ => {
	render(<UI />, document.getElementById("container"));
})();
