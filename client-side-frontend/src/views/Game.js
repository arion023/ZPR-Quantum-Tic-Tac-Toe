import Board from "../components/Board";
import { useLocation, useParams } from "react-router-dom";
import axios from "axios";

import "../stylesheets/Game.scss"
import { useState, useEffect } from "react";


function Game( props ) {

    console.log("redner")

    const location = useLocation();
    const [boardAction, setMove ] = useState([]);

    const {gameId} = useParams();


    const [boardState, setBoard] = useState([["Error"]]);
    const [player, setPlayer] = useState('X');

    useEffect(() => { 
        console.log("")
        getBoard(); 
    }, [])

    useEffect(() => { 
        console.log("onMount")
        getBoard(); 
    }, [])


    

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

    function getBoard() {
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
                    let tmpBoard = response.data.board;
                    setBoard(parseBoard(tmpBoard));
                    console.log("response");
                    console.log(response.data);
                    console.log("board");
                    console.log(tmpBoard);
                    setMove([])
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
            <h2>turn = {player}</h2>
            <h2>status = {player}</h2>
            <div className="container">
                <Board board={boardState} boardAction={boardAction} setMove={setMove } playerSign={player} />
            </div>
            <div className="container">
                <button id="make-move" onClick={() => make_move()} className="btn btn-outline-warning btn-lg">Make move</button>
            </div>
        </div>
    )
}


export default Game;
