# SmartSwitch
A website for a smart home system using which people can control the electric appliances in their homes without having to do any electrical wiring work. 

The website is made using a flask webserver and html, css and js frontend. The SmartSwitch works by pushing or pulling the switch using a dc motor. The motor is controlled by a NodeMCU using an L298N H bridge motor driver. 

I couldn't find a way to make it work with websockets so instead the switch sends post requests to the server which returns the state of the switch from the json file. The JSON file is edited when the user clicks on or off from the website. 

Obviously there is probably a much safer and better way to do this but meh, this is what I came up with in the time I had.