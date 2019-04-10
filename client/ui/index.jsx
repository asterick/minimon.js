import React, { Component } from "react";
import Button from '@material-ui/core/Button';

class FormContainer extends Component {
	constructor() {
		super();

		this.state = {
			title: ""
		};
	}
	render() {
		return (
			<Button variant="contained" color="primary">
      			Hello World
    		</Button>
		);
	}
}

export default FormContainer;
