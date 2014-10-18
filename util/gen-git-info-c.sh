echo "*** Generating Git Info"
short_sha=`git rev-parse --short HEAD`
cat > $1 <<-EOF
#ifndef GIT_INFO_H
#define GIT_INFO_H
#define BUILD_SOURCE "git"
#define GIT_SHORT_HASH "$short_sha"
#endif
EOF
