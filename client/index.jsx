import { render } from 'inferno';
import UI from  "./ui/index.jsx";

(async _ => {
	render(<UI />, document.getElementById("container"));
	start();
})();
