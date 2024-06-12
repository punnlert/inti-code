import React from 'react';
import { useEffect, useState } from 'react'
import { io } from 'socket.io-client';
import { ChildList } from './components/ChildList.jsx';
import './App.css'

const socket = io('http://localhost:3000');

function App() {
  const [displayText, setDisplayText] = useState("Not Connected");
  const [page, changePage] = useState("src/assets/body.svg");
  const [displayList, toggleDisplay] = useState(true);
  const [show, changeShow] = useState(false);

  // socket.on('connect', (arg) => {
  //   setDisplayText("connected with the server");
  //   console.log('connected with the server');
  // })

  socket.on('button data', (data) => {
    changeShow(true);
  })

  const handleClick = () => {
    // document.getElementsByClassName("piccontainer")[0].style.content = "url(./assets/body.svg);"
    // socket.emit("sendChildSignal", []);
    if (displayList) {
      changePage("src/assets/child-page.svg");
      toggleDisplay(false);
    } else {
      changePage("src/assets/body.svg");
      toggleDisplay(true);
    }
    // changeShow(true);
  }

  const disablepopup = () => {
    changeShow(false);
  }

  return (
    <>
      <div className='appcontainer' >
        {show ? (<div className="darken" style={{opacity: 1}} onClick={disablepopup}>
          <div className="alert" style={{content: `url(src/assets/alert.svg)`}} onClick={disablepopup}></div>
        </div>) : (null)}
        
        <div className="navbarcontainer">

        </div>
        <div className="contentcontainer" onClick={handleClick}>
          <div className="piccontainer" style={{content: `url(${page})`}}></div>
          {/* <div className="childlistcontainer">
            <h2>Abused Children List</h2>
            <div className="tablecontainer">
              <button onClick={handleClick}>send signal</button>
              <ChildList CaseManagement={null} Name={null} Sex={null} DOB={null} Status={null}/>
            </div>
          </div> */}
        </div>

      </div>
    </>
  )
}

export default App
