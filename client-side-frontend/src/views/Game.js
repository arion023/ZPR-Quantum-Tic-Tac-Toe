import Board from "../components/Board";
import { useLocation, useParams } from "react-router-dom";
import axios from "axios";

import "../stylesheets/Game.scss"
import { useState, useEffect } from "react";


function Game( props ) {

    const location = useLocation();
    const [boardAction, setBoardAction] = useState([]);

    const {gameId} = useParams();

    let tmpBoard = []
    if(location.state != null)
    {
        console.log("init parse board")
        tmpBoard=parseBoard(location.state.board);
    }
    const [boardState, setBoard] = useState(tmpBoard);
    const [player, setPlayer] = useState(location.state.currentPlayer);
    

    function reload()
    {
        //TODO auto reload
        getBoard();
    }

    function parseBoard(boardDict)
    {
        var newBoard = new Array();
        console.log("boardDict")
        console.log(boardDict)
        for(let i in boardDict)
        {
            if (boardDict[i].entanglements != null)
            {
                console.log("boardDict")
                console.log(boardDict)
            
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
        //console.log("board");
        //console.log(newBoard);
        return newBoard;
    }

    function getBoard()
    {
        axios.get('/get_game/' + gameId)
        .then(function (response) {
            // handle success
            setBoard(parseBoard(response.data.board))
        })
        .catch(function (error) {
           // TODO
           console.log(error);
        })
    }

    function make_move()
    {
        if(boardAction.length != 2)
            alert("Invalid move!");
        else
        {
            console.log("response");

            //TODO: error handling
            axios.post('/games/'+gameId+'/MakeMove', {
                player: (player === "X") ? 1 : 2,
                idx1: boardAction[0],
                idx2: boardAction[1]
                  })
                  .then(function (response) {
                    let tmpBoard = response.data.board
                    setBoard(parseBoard(tmpBoard))
                    console.log("response");
                    console.log(response.data);
                    console.log("board");
                    console.log(tmpBoard);
                    setBoardAction([])
                    setPlayer(response.data.currentPlayer);
                  })
                  .catch(function (error) {
                    console.log(error);
                  });

        }
    }


    return (
        <div className='App Game'>
            <h1>Game ID: {gameId} </h1>
            <h2>PLAYER = {player}</h2>
            <div className="container">
                <Board board={boardState} boardAction={boardAction} setBoardAction={setBoardAction} playerSign={player} />
            </div>
            <div className="container">
                <button id="make-move" onClick={() => make_move()} className="btn btn-outline-warning btn-lg">Make move</button>
            </div>
            <div className="container">
                <button onClick={() => reload(setBoard)} className="btn btn-danger btn-lg">Reload</button>
            </div>
        </div>
    )
}


export default Game;
