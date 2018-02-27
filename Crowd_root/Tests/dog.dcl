  typedef <data>  {
                     double  energy ;
                  } ;

      <data>  data ;

//------------------------------- Отложенный запрос себя или системное сообщение

  if($MsgSender=="") 
  {
//- - - - - - - - - - - - - - - - Системные сообщения: инициализация и т.д.

     if($MsgType=="system")
     {
          data.energy = 0.8 ; 

          data.StateSave() ;
            return ;
     }
//- - - - - - - - - - - - - - - - Отложенные запросы на себя

//- - - - - - - - - - - - - - - - 
  }
//------------------------------- Внешнее сообщение

  else
  {
     data.StateRead() ;

    $ThisColor<=="000:255:255" ;
    $ThisX     = $ThisX+(ObjectXYZ($MsgSender, "X")-$ThisX)*data.energy ;
    $ThisY     = $ThisY+(ObjectXYZ($MsgSender, "Y")-$ThisY)*data.energy ;

     if($Step>14) data.energy = data.energy*0.95 ; 

     data.StateSave() ;

  }

return ;
