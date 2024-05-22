import { useState } from 'react'
import { io } from 'socket.io-client';
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'

const socket = io('http://localhost:3000');

function App() {
  const [count, setCount] = useState(0);
  const [displayText, setDisplayText] = useState("Not Connected");

  socket.on('connect', (arg) => {
    setData("connected with the server");
    console.log('connected with the server');
  })

  socket.on('button data', (data) => {
    setDisplayText(data);
    console.log(data);
  })

  return (
    <>
      <div className="card">
          <h1> {displayText} </h1>
      </div>
    </>
  )
}

export default App
