'use strict';
var common = require('../common');
var assert = require('assert');

if (!common.hasCrypto) {
  console.log('1..0 # Skipped: missing crypto');
  return;
}
/* This test uses only ECDHE-RSA-RC4-SHA, this means:
 * Key Exchange: ECDH
 * Encryption: RC4
 * Bits: 128
 * The RFC name is TLS_ECDHE_RSA_WITH_RC4_128_SHA (cf. https://testssl.sh/openssl-rfc.mappping.html)
 * FIPS does NOT support RC4, so we should skip this test. 
 */
if (common.hasFipsCrypto) {
  console.log('1..0 # Skipped: FIPS 140-2 does not support RC4 cipher');
  return;
}

var tls = require('tls');

var exec = require('child_process').exec;
var fs = require('fs');

var options = {
  key: fs.readFileSync(common.fixturesDir + '/keys/agent2-key.pem'),
  cert: fs.readFileSync(common.fixturesDir + '/keys/agent2-cert.pem'),
  ciphers: '-ALL:ECDHE-RSA-RC4-SHA', /* Disable ALL but ECDHE-RSA-RC4-SHA */
  ecdhCurve: 'prime256v1'
};

var reply = 'I AM THE WALRUS'; // something recognizable
var nconns = 0;
var response = '';

process.on('exit', function() {
  assert.equal(nconns, 1);
  assert.notEqual(response.indexOf(reply), -1);
});

var server = tls.createServer(options, function(conn) {
  conn.end(reply);
  nconns++;
});

server.listen(common.PORT, '127.0.0.1', function() {
  var cmd = '"' + common.opensslCli + '" s_client -cipher ' + options.ciphers +
            ' -connect 127.0.0.1:' + common.PORT;

  // for the performance and stability issue in s_client on Windows
  if (common.isWindows)
    cmd += ' -no_rand_screen';

  exec(cmd, function(err, stdout, stderr) {
    if (err) throw err;
    response = stdout;
    server.close();
  });
});
