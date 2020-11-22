# PROTOCOL

## Sending command

### Client Side 

> `client` Sends command as string    
> `client` accepts `acknowledgement` for the command    
> `client` sends ready signal to accept command response  
> `client` accepts command response   
> `client` sends `acknowledgement` for completion of accepting process  
> `client` recieves `acknowledgement` from `server` that command is over


### Server side 

> `server` accepts the command    
> `server` sends `acknowledgement` for that command is accepted     
> `server` interprets the command     
> `server` accepts the ready signal for the command   
> `server` sends command response to `client`   
> `server` accepts the completion `acknowledgement` to the `client`   
> `server` sends command completion `acknowledgement` to `client`

## ERROR 

Currently some commands are being read and then server side shows error on line `96` where it is freeing the string, it shows nmap chunk invalid pointer error.

Debug on 17 November