import React from 'react';
import { useEffect, useState } from 'react'
import { io } from 'socket.io-client';
import { ChildList } from './components/ChildList.jsx';
import './App.css'

// const socket = io('http://localhost:3000');

function App() {
  const [displayText, setDisplayText] = useState("Not Connected");

  // socket.on('connect', (arg) => {
  //   setDisplayText("connected with the server");
  //   console.log('connected with the server');
  // })

  // socket.on('button data', (data) => {
  //   setDisplayText(data);
  //   const input = JSON.parse(data);
  //   console.log(input.Event);
  // })

  return (
    <>
      <div className='appcontainer'>
        <div className="navbarcontainer">

        </div>
        <div className="contentcontainer">
          <div className="childlistcontainer">
            <h2>Abused Children List</h2>
            {/* <div className="tablecontainer">

            </div> */}
            <ChildList CaseManagement={null} Name={null} Sex={null} DOB={null} Status={null}/>
          </div>
        </div>
      </div>
    </>
  )
}

export default App
