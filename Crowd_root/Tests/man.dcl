<unknown>  Links ;
     char  dummy ; 
      int  i ;

//------------------------------ Отложенный запрос себя 

  if($MsgSender=="") 
  {

     if($MsgKind=="Clear color")  $ThisColor<=="255:255:255" ;

  }
//------------------------------ Внешнее сообщение

  else
  {
               $ThisColor<=="255:000:000" ;

          Links.GetLinks("pair","pass",$MsgSender,"none") ;

     for(i=0 ; i<Links.count ; i++) {
         SendMessage(Links[i].partner, "Contact", "Call", "") ;
                                    }

         SendMessage("dog", "Contact", "Atu!", "") ;
              Recall("Clear color") ;

  }

return ;
