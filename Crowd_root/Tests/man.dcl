<unknown>  Links ;
     char  dummy ; 
      int  i ;

//------------------------------- Отложенный запрос себя или системное сообщение

  if($MsgSender=="") 
  {
//- - - - - - - - - - - - - - - - Системные сообщения: инициализация и т.д.
     if($MsgType=="system")
     {
            return ;
     }
//- - - - - - - - - - - - - - - - Отложенные запросы на себя
     if($MsgKind=="Clear color")  $ThisColor<=="255:255:255" ;

            return ;
//- - - - - - - - - - - - - - - - 
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
