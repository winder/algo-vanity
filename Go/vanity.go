package main

import (
	"flag"
	"fmt"
	"strings"

	"github.com/algorand/go-algorand-sdk/crypto"
	"github.com/algorand/go-algorand-sdk/mnemonic"
)

func search(pre string, out chan<- crypto.Account) {
	for true {
		acct := crypto.GenerateAccount()
		if strings.HasPrefix(acct.Address.String(), pre) {
			out <- acct
		}
	}
}

func main() {
	var workers int
	var prefix string

	flag.IntVar(&workers, "workers", 14, "Number of worker routines to spawn.")
	flag.StringVar(&prefix, "prefix", "", "prefix to search for.")

	flag.Parse()

	upperPre := strings.ToUpper(prefix)
	fmt.Printf("Starting %d workers searching for %s\n", workers, upperPre)
	acctChan := make(chan crypto.Account)

	for i := 0; i < workers; i++ {
		go search(upperPre, acctChan)
	}

	for acct := range acctChan {
		m, _ := mnemonic.FromPrivateKey(acct.PrivateKey)
		fmt.Printf("%s - %s\n", acct.Address.String(), m)
	}
}
