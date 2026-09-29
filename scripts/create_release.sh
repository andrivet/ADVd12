#!/usr/bin/env zsh

: '
Create a new release.
'

set -euo pipefail

if [[ "$OSTYPE" != darwin* ]]; then echo "Works only on macOS, sorry" ; exit 1 ; fi

function pause(){
   read -r -s -k "?$*"$'\n'
}

version="1.0.0"
scripts="${0:A:h}"
mkdir -p "${scripts}/../../releases/v${version}"
release="$(cd "${scripts}/../../releases/v${version}" && pwd)"
lcd="$(cd "${scripts}/../lcd" && pwd)"

echo "======================================================="
echo "Create ADVd12 ${version}"
echo "======================================================="
echo
echo "BE SURE YOU HAVE CHANGED:"
echo "- CHANGELOG.md"
echo "- bump-my-version <part>"
echo "- User Manual if necessary"
echo
pause 'Press any key to continue or Ctrl-C to abort...'

echo
echo "**** Generate assets..."
python "${scripts}/_adv_encode_assets.py" "${lcd}/png" --outdir "${lcd}/assets"
echo

"${scripts}/_create-release-image.sh" ${version} advd12_230_mks12
"${scripts}/_create-release-image.sh" ${version} advd12_230_mks12_touch
"${scripts}/_create-release-image.sh" ${version} advd12_230_mks13
"${scripts}/_create-release-image.sh" ${version} advd12_230_mks13_touch
