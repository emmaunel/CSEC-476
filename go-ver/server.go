package main

import (
	"fmt"
	"net"
	"os"
)

const(
	CONN_HOST = "localhost"
    CONN_PORT = "3333"
	CONN_TYPE = "tcp"
	COMMAND = "a"
	DOWNLOAD = "d"
	PING = "p"
	SHELL = "s"
)

func handleConnetion(conn net.Conn){
	conn.Write([]byte("Golang write"))
	conn.Close()
}

func main(){
	l, err := net. Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)

	if err != nil{
		fmt.Println("Error listening")
		os.Exit(0)
	}

	defer l.Close()
	fmt.Println("Listening on " + CONN_HOST + ":" + CONN_PORT)

	for{
		conn, err := l.Accept()
		if err != nil {
			fmt.Println("Error accepting")
			os.Exit(0)
		}

		go handleConnetion(conn)
	}
}