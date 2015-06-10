

if [ -z "$1" ];
then
    echo "pass user account. eg ~/dev_setup <username>"
    exit
fi

echo 'alias jl="vim ~/webq/journal/`date --iso`"' >> ~/.aliases.sh

sudo cat << EOF >> /etc/hosts
10.129.49.76	server
10.129.41.67	check
10.129.26.130   gen
10.129.2.55 vacha
EOF

cat << EOF >> .ssh/config
Host server
	HostName 10.129.49.76
	User	$1

Host check
	HostName 10.129.41.67
	User	$1

Host gen
	HostName 10.129.26.130
	User	$1

Host vacha
	HostName 10.129.2.55
	User	$1
EOF
