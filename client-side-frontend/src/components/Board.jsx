import { click } from "@testing-library/user-event/dist/click";
import axios from "axios";
import { useEffect } from "react";

function Board( {board, boardAction, setMove, playerSign} ) {

    const n = Math.sqrt(board.length);
    let tmpBoard = board.slice();

    function markTile(idx)
    {
        console.log(boardAction);
        if( boardAction.length === 0)
        {
            setMove([ idx ]);
        }
        else if(boardAction.length === 1)
        {
            if(boardAction.includes(idx))
            {
                setMove([]);
            }
            else
            {
                let tmpBoardAction = boardAction.slice();
                tmpBoardAction.push(idx);
                setMove(tmpBoardAction);
            }
        } else
        {
            if(boardAction.includes(idx))
            {
                setMove([]);
            }
        }
    };

    function renderTile(idx)
    {
        if( boardAction.includes(idx) )
        {
            return board[idx] + playerSign;
        }
        return board[idx];
    };

    function renderRow(rowTiles, row)
    {
        return (
            <div className="row-container">
            {rowTiles.map( (tile, idx) => (
                <button key={idx} className="tic-tac-toe-tile" onClick={() => markTile(row*n + idx)}>
                    {renderTile(row*n + idx)}
                </button>
            ))}
            </div>
        )
    }

    let rows = [];
    for(let i = 0; i<=board.length-n; i=i+n)
    {
        let row = board.slice(i, i+n);
        rows.push(row);
    }

    return (
        <div className="container">
            <div className="board-containter">
                {rows.map( (row, idx) => renderRow(row, idx))}
            </div>
        </div>
    );
}

export default Board;