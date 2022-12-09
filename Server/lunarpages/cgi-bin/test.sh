# Encode strings we send to server
# http://stackoverflow.com/questions/296536/how-to-urlencode-data-for-curl-command
enc() {
  local string="${1}"
  local strlen=${#string}
  local encoded=""
  local pos c o

  for (( pos=0 ; pos<strlen ; pos++ )); do
     c=${string:$pos:1}
     # encode EVERY char
     printf -v o '%02x' "'$c"
     encoded+="${o}"
  done
  echo "${encoded}"    # You can either set a return variable (FASTER) 
  REPLY="${encoded}"   #+or echo the result (EASIER)... or both... :p
}

###enc "Hello"

echo

echo "************************************************************************************"
echo "TEST: Log a hi score"
score=`enc "12345"`
###echo $score
url="www.amju.com/cgi-bin/rd/rd_log_hi_score.pl?score='$score'&level='31'&device_id='313233343536'&depth='3132'&nick='56575859'&x='30'&y='31'&z='32'"
echo $url
curl -g $url

#"www.amju.com/cgi-bin/rd/rd_log_hi_score.pl?score='" + enc "12345" + "'&level='1'&device_id='313233343536'&depth='3132'&nick='56575859'&x='30'&y='31'&z='32'"
echo
exit

echo "************************************************************************************"
echo "TEST: Get hi scores: should return XML of hi score table"
curl -g "www.amju.com/cgi-bin/rd/rd_req_hi_scores.pl"
echo

echo "************************************************************************************"
echo "TEST: Set a new unique device"
curl -g "www.amju.com/cgi-bin/rd/rd_log_device.pl?device_id='313233343536'&device_user_name='4546474748'&device_model='asdf'&device_os_version='4.5.6'&device_client_version='1.1'&device_manufacturer='jctest'"
echo
echo "************************************************************************************"
echo "TEST: Set a new session"
curl -g "www.amju.com/cgi-bin/rd/rd_log_play_session.pl?device_id='313233343536'&session_start='1000'&session_end='2000'&session_level='1'&session_depth='323334'&session_score='30'"
echo

echo "************************************************************************************"


