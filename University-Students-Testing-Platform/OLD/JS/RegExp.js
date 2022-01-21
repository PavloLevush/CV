var login = document.getElementById('usr')

function logins(login){
var numeric = /^[0-9]{7}$/;
if(login.value.match(numeric)){
	return true;
}else{
	alert("login is incorect");
	login.focus();
	return false;
}
}