--[[
  Project: TinyAd
  Description: TinyAd Protocol Dissector Plugin for Wireshark
  Author: Kamil Przybyła
  Created: 31.05.2021
]]

tinyad_protocol = Proto("TinyAd",  "TinyAd Protocol")

-- General
message_type = ProtoField.uint8("tinyad.message_type", "messageType", base.DEC)
message_id = ProtoField.string("tinyad.id", "id", base.UNICODE)

-- TYPE_MULTIMEDIA_HEADER
message_filesize = ProtoField.uint32("tinyad.filesize", "fileSize", base.DEC)
message_num_chunks = ProtoField.uint32("tinyad.num_chunks", "numChunks", base.DEC)
message_filename = ProtoField.string("tinyad.filename", "filename", base.UNICODE)

-- TYPE_MULTIMEDIA_DATA
message_chunk_num = ProtoField.uint32("tinyad.chunk_num", "chunkNum", base.DEC)
message_crc = ProtoField.uint32("tinyad.crc", "crc", base.DEC)

-- TYPE_CONTROL_MESSAGE
message_panel_id = ProtoField.string("tinyad.panel_id", "panelId", base.UNICODE)
message_control_msg = ProtoField.uint8("tinyad.control_msg", "controlMsg", base.DEC)
-- message_id
-- message_chunk_num

-- TYPE_PANEL_HELLO
-- message_panel_id
message_panel_config = ProtoField.string("tinyad.panel_config", "panelConfig", base.UNICODE)

-- TYPE_SERVER_CONN_ACCEPT
-- message_panel_id
message_broadcast_address = ProtoField.string("tinyad.broadcast_address", "broadcastAddress", base.UNICODE)

tinyad_protocol.fields = { 
  message_type, message_id, -- general
  message_filesize, message_num_chunks, message_filename, -- TYPE_MULTIMEDIA_HEADER
  message_chunk_num, message_crc, -- TYPE_MULTIMEDIA_DATA
  message_panel_id, message_control_msg, -- TYPE_CONTROL_MESSAGE
  message_panel_config, -- TYPE_PANEL_HELLO
  message_broadcast_address --TYPE_SERVER_CONN_ACCEPT
 }

function get_msg_type_name(type)
  local type_name = "Unknown"

  if type == 0 then 
    type_name = "TYPE_MULTIMEDIA_DATA"
  elseif type == 1 then 
    type_name = "TYPE_MULTIMEDIA_HEADER"
  elseif type == 2 then 
    type_name = "TYPE_CONTROL_MESSAGE"
  elseif type == 3 then 
    type_name = "TYPE_PANEL_HELLO"
  elseif type == 4 then 
    type_name = "TYPE_SERVER_CONN_ACCEPT"
  end

  return type_name
end

function get_control_msg_name(type)
  local control_msg_name = "Unknown"

  if type == 0 then 
    control_msg_name = "CONTROL_NAK"
  elseif type == 1 then 
    control_msg_name = "CONTROL_FILE_HEADER_REQUESTED"
  elseif type == 2 then
    control_msg_name = "CONTROL_SLOW_DOWN_TRANSMISSION"
  elseif type == 3 then
    control_msg_name = "CONTROL_FILE_NOT_FOUND"
  end

  return control_msg_name
end

function tinyad_protocol.dissector(buffer, pinfo, tree)
  length = buffer:len()
  if length == 0 then 
    return 
  end

  pinfo.cols.protocol = tinyad_protocol.name

  local subtree = tree:add(tinyad_protocol, buffer(), "TinyAd Protocol")

  local msg_type = buffer(0,1):uint()
  local type_name = get_msg_type_name(msg_type)
  subtree:add_le(message_type, buffer(0,1)):append_text(" (" .. type_name .. ")")

  if type_name == "TYPE_MULTIMEDIA_HEADER" then
    -- For some reason buffer(1,8):uint64():tonumber() does not work
    -- so I take only the lower half of filename size
    -- it should be small anyway
    local id_str_len = buffer(1,4):le_uint()
    subtree:add_le(message_id, buffer(9,id_str_len))

    subtree:add_le(message_filesize, buffer(9+id_str_len,4))
    subtree:add_le(message_num_chunks, buffer(9+id_str_len+4,4))

    local str_len = buffer(9+id_str_len+8,4):le_uint()
    subtree:add_le(message_filename, buffer(9+id_str_len+16,str_len))

  elseif type_name == "TYPE_MULTIMEDIA_DATA" then
    local id_str_len = buffer(1,4):le_uint()
    subtree:add_le(message_id, buffer(9,id_str_len))

    subtree:add_le(message_chunk_num, buffer(9+id_str_len,4))
    subtree:add_le(message_crc, buffer(length-4,4))

  elseif type_name == "TYPE_CONTROL_MESSAGE" then
    local str_len = buffer(1,4):le_uint()
    subtree:add_le(message_panel_id, buffer(9,str_len))

    local control_msg_type = buffer(9+str_len,1):uint()
    local msg_name = get_control_msg_name(control_msg_type)
    subtree:add_le(message_control_msg, buffer(9+str_len,1)):append_text(" (" .. msg_name .. ")")

    local id_str_len = buffer(9+str_len+1,4):le_uint()
    subtree:add_le(message_id, buffer(9+str_len+9,id_str_len))
    subtree:add_le(message_chunk_num, buffer(9+str_len+9+id_str_len,4))

  elseif type_name == "TYPE_PANEL_HELLO" then
    local panel_id_str_len = buffer(1,4):le_uint()
    subtree:add_le(message_panel_id, buffer(9,panel_id_str_len))

    local panel_config_str_len = buffer(9+panel_id_str_len,4):le_uint()
    subtree:add_le(message_panel_config, buffer(9+panel_id_str_len+8,panel_config_str_len))
  
  elseif type_name == "TYPE_SERVER_CONN_ACCEPT" then
    local panel_id_str_len = buffer(1,4):le_uint()
    subtree:add_le(message_panel_id, buffer(9,panel_id_str_len))

    local broadcast_address_str_len = buffer(9+panel_id_str_len,4):le_uint()
    subtree:add_le(message_broadcast_address, buffer(9+panel_id_str_len+8,broadcast_address_str_len))
  end
end

local udp_port = DissectorTable.get("udp.port")
udp_port:add(21337, tinyad_protocol)
udp_port:add(21345, tinyad_protocol)