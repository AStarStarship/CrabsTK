

    if (delta_t <= expr->timeout_us) {
        if (delta_t < 0)    //< Special case for Epoch (rare)
            delta_t *= -1;
    }

    bin_begin  = BInBegin (bin);
    bin_end    = bin_begin + size;
    bin_start  = bin_begin + bin->start;
    bin_stop   = bin_begin + bin->stop;
    space  = SlotSpace (bin_start, bin_stop, size);
    length = size - space;
    #if SCRIPT_DEBUG == SCRIPT_EXPR
    Text<> out;
    COut (out << "\n| Scanning Expr:0x" << out.Pointer (expr)
          << " with length:" << length);
    #endif
    for (; length != 0; --length) {
        b = *bin_start;
        *slot_start++ = b;
        #if SCRIPT_DEBUG == SCRIPT_EXPR
        COut (out << out.Line ('=')
              << "\n| " << length << ":\'" << out.Clear ();
              << ((b < 32 || b == 127) ? (out << AsciiText ((AsciiCode)b)) 
                                       : (out << b))
              << "\' " << BInState ()[bin_state] << " state"
                  << out.Line ()));
        #endif

        if (++bin_start >= bin_end) bin_start -= size;
        // Process the rest of the bytes in a loop to reduce setup overhead.
        switch (bin_state) {
            case BIn::AddressState: {
                hash = Hash16 (b, hash);
                #if SCRIPT_DEBUG == SCRIPT_EXPR
                COut (out << "\n| hash:" << out.Hex (hash));
                #endif
                // When verifying an address, there is guaranteed to be an
            