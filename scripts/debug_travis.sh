
travis login --com

token=$(travis token --com --no-interactive)

travis-debug() {
curl -s -X POST \
  -H "Content-Type: application/json" \
  -H "Accept: application/json" \
  -H "Travis-API-Version: 3" \
  -H "Authorization: token $token" \
  -d "{\"quiet\": true}" \
  https://api.travis-ci.com/job/$1/debug
}

