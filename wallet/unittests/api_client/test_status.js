var net = require('net');

var client = new net.Socket();
client.connect(10000, '127.0.0.1', function() {
	console.log('Connected');
	client.write(JSON.stringify(
		{
			jsonrpc: '2.0',
			id: 123,
			method: 'tx_status',
			params: 
			{
				'txId' : '10c4b760c842433cb58339a0fafef3db',
			}
		}) + '\n');
});

client.on('data', function(data) {

	var res = JSON.parse(data);

	console.log("got:", res);

	client.destroy(); // kill client after server's response
});

client.on('close', function() {
	console.log('Connection closed');
});
