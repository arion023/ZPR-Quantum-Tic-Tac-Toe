import Board from "../components/Board";
import { useLocation, useParams } from "react-router-dom";
import axios from "axios";

import "../stylesheets/Game.scss"
import { useState, useEffect } from "react";


function Game( props ) {

    console.log("render")

    const location = useLocation();

    const {gameId} = useParams();
    
    const [response, setResponse] = useState("None");
    const [boardState, setBoard] = useState([["Error"]]);
    const [gameStatus, setGameStatus] = useState("Off");
    const [player, setPlayer] = useState('-');
    const [cycle, setCycle] = useState([]);
    
    const [move, setMove ] = useState([]);
    
    //load game on init
    useEffect(() => { 
        console.log("onMount")
        getGame(); 
    }, [])

    //move validation
    useEffect(() => { 
        if (gameStatus == "cycle" )
        {
            console.log("cycle");
            console.log(cycle);
            if (move.length > 1)
            {
                setMove(move.slice(-1));
            }
        }
    }, [move])

    //update response
    useEffect(() => {
        console.log("response");
        console.log(response);
        if(response != "None")
        {
            setBoard(parseBoard(response.data.board));
            setPlayer(response.data.currentPlayer);
            setGameStatus(response.data.status);
            setMove([])
            if(gameStatus == "cycle")
            {
                setCycle(response.data.cycle);    
            }
        }       
    }, [response])
    

    function parseBoard(boardDict)
    {
        var newBoard = new Array();
        console.log("boardDict")
        console.log(boardDict)
        for(let i in boardDict)
        {
            if (boardDict[i].entanglements != null)
            {
            
                let signs = ""
                let ent = boardDict[i].entanglements
                for(let e_id in ent)
                {
                    signs+=ent[e_id] + e_id + " ";
                }
                newBoard.push(signs)
            }
            else
            {
                newBoard.push(boardDict[i].sign);
            }
        }
        return newBoard;
    }

    function getGame() {
        axios.get('/get_game/' + gameId)
        .then(function (response) {
            // handle success
            setResponse(response);
        })
        .catch(function (error) {
           // TODO
           console.log(error);
        })
    }


    function postCycle()
    {
        alert("Invalid move!");
    }


    function postMove()
    {
        let tiles_idx = move;

        if(gameStatus === "cycle" && move.length === 1)
        {
            tiles_idx.push(-1);
        }

        if(tiles_idx.length != 2)
            alert("Invalid move!");
        else
        {
            console.log("response");

            //TODO: error handling
            axios.post('/games/'+gameId+'/MakeMove', {
                player: player,
                idx1: tiles_idx[0],
                idx2: tiles_idx[1]
                  })
                  .then(function (response) {
                    setResponse(response);
                  })
                  .catch(function (error) {
                    console.log(error);
                  });
        }
    }


    return (
        <div className='App Game'>
            <h1>Game ID: {gameId} </h1>
            <h2>turn = {player}</h2>
            <h2>status = {gameStatus}</h2>
            <div className="container">
                <Board board={boardState} move={move} setMove={setMove} playerSign={player} />
            </div>
            <div className="container">
                <button id="make-move" onClick={() => postMove()} className="btn btn-outline-warning btn-lg">Make move</button>
            </div>
        </div>
    )
}


export default Game;
